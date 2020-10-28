#include <regex.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "string_util.h"

// Declarations

static int STATEMENT(char *, size_t *, struct statement *);

static int COMMAND(char *, size_t *, struct statement *);

static int ARGS(char *, size_t *, struct command *);

static int ALIAS_ARGS(char *, size_t *, struct command *);

static int EXT_ARGS(char *, size_t *, struct command *);

static int PIPE(char *, size_t *, struct command *);

// Definitions

static int STATEMENT(char *input, size_t *i, struct statement *statement)
{
	int error_code = 0;
	regmatch_t match;
	if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		i[0] += match.rm_eo;
	if (!regexec(&regex_end, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		return error_code;
	}
	else error_code = COMMAND(input, i, statement);
	if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		i[0] += match.rm_eo;
	if (!regexec(&regex_end, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		return error_code;
	}
	else
	{
		printf("[Syntax Error] Expecting semicolon or newline.\n");
		i[0] = strlen(input);
		return 1;
	}
}

static int COMMAND(char *input, size_t *i, struct statement *statement)
{
	char *command_name;
	regmatch_t match;
	if (!regexec(&regex_built_in, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		command_name = substring(input, i[0], i[0] + match.rm_eo);
		statement->first = new_command();
		statement->first->name = command_name;
		add_arg(statement->first, command_name);
		i[0] += match.rm_eo;
		return ARGS(input, i, statement->first);
	}
	else if (!regexec(&regex_alias_cmd, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		command_name = substring(input, i[0], i[0] + match.rm_eo);
		statement->first = new_command();
		statement->first->name = command_name;
		add_arg(statement->first, command_name);
		i[0] += match.rm_eo;
		return ALIAS_ARGS(input, i, statement->first);
	}
	else if (!regexec(&regex_external, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		command_name = substring(input, i[0], i[0] + match.rm_eo);
		statement->first = new_command();
		statement->first->name = command_name;
		add_arg(statement->first, command_name);
		i[0] += match.rm_eo;
		return EXT_ARGS(input, i, statement->first);
	}
	else
	{
		printf("[Syntax Error] Expecting command.\n");
		i[0] = strlen(input);
		return 2;
	}
}

static int ARGS(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&regex_argument, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			add_arg(cmd, substring(input, i[0], i[0] + match.rm_eo));
			i[0] += match.rm_eo;
			return ARGS(input, i, cmd);
		}
		else
		{
			printf("[Syntax Error] Expecting argument for built-in command.\n");
			i[0] = strlen(input);
			return 3;
		}
	}
	return 0;
}

static int ALIAS_ARGS(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		size_t whitespace_rm_eo = match.rm_eo;
		i[0] += whitespace_rm_eo;
		if (!regexec(&regex_alias, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			add_arg(cmd, substring(input, i[0], i[0] + match.rm_eo));
			i[0] += match.rm_eo;
		}
		else
		{
			i[0] -= whitespace_rm_eo;
			return ARGS(input, i, cmd);
		}
	}
	return 0;
}

static int EXT_ARGS(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&regex_argument, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			add_arg(cmd, substring(input, i[0], i[0] + match.rm_eo));
			i[0] += match.rm_eo;
			return EXT_ARGS(input, i, cmd);
		}
		else if (!regexec(&regex_redirection, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			char redirection_symbol = input[i[0]];
			i[0] += match.rm_eo;
			if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
			{
				i[0] += match.rm_eo;
				if (!regexec(&regex_file_path, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
				{
					if (redirection_symbol == '<')
						cmd->input = substring(input, i[0], i[0] + match.rm_eo);
					else
						cmd->output = substring(input, i[0], i[0] + match.rm_eo);
					i[0] += match.rm_eo;
					return EXT_ARGS(input, i, cmd);
				}
				else
				{
					printf("[Syntax Error] Expecting file for redirection.\n");
					i[0] = strlen(input);
					return 5;
				}
			}
			else
			{
				printf("[Syntax Error] Expecting whitespace.\n");
				i[0] = strlen(input);
				return 4;
			}
		}
		else return PIPE(input, i, cmd);
	}
	return 0;
}

static int PIPE(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&regex_pipe, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&regex_whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			i[0] += match.rm_eo;
			if (!regexec(&regex_external, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
			{
				cmd->pipe = new_command();
				cmd->pipe->name = substring(input, i[0], i[0] + match.rm_eo);
				i[0] += match.rm_eo;
				return EXT_ARGS(input, i, cmd->pipe);
			}
			else
			{
				printf("[Syntax Error] Expecting external command for pipe.\n");
				i[0] = strlen(input);
				return 7;
			}
		}
		else
		{
			printf("[Syntax Error] Expecting whitespace.\n");
			i[0] = strlen(input);
			return 6;
		}
	}
	return 0;
}

void compile_regex()
{
	regcomp(&regex_whitespace, "[\t ]+", REG_EXTENDED);
	regcomp(&regex_built_in, "cd|exit|path|myhistory", REG_EXTENDED);
	regcomp(&regex_alias_cmd, "alias", REG_EXTENDED);
	regcomp(&regex_alias, "[A-Za-z_][A-Za-z0-9_]*='.*'", REG_EXTENDED);
	regcomp(&regex_external,
			"^((~|\\.|\\.\\.)\\/?|\\/)?[A-Za-z0-9_][\\.A-Za-z0-9_-]*(\\/[A-Za-z0-9_][\\.A-Za-z0-9_-]*)*",
			REG_EXTENDED);
	regcomp(&regex_argument, "[^\t ;\n<>|'\"\\]+", REG_EXTENDED);
	regcomp(&regex_redirection, "[<>]", REG_EXTENDED);
	regcomp(&regex_file_path,
			"^((~|\\.|\\.\\.)\\/?|\\/)?[A-Za-z0-9_][\\.A-Za-z0-9_-]*(\\/[A-Za-z0-9_][\\.A-Za-z0-9_-]*)*",
			REG_EXTENDED);
	regcomp(&regex_pipe, "[|]", REG_EXTENDED);
	regcomp(&regex_end, "[;\n]|(^$)|(;[\t ]*\n)", REG_EXTENDED);
}

void free_regex()
{
	regfree(&regex_whitespace);
	regfree(&regex_built_in);
	regfree(&regex_alias_cmd);
	regfree(&regex_alias);
	regfree(&regex_external);
	regfree(&regex_argument);
	regfree(&regex_redirection);
	regfree(&regex_file_path);
	regfree(&regex_pipe);
	regfree(&regex_end);
}

struct statement *parse(char *input)
{
	// A pointer to the input's current index.
	size_t *i = malloc(sizeof(size_t));
	i[0] = 0;
	// A pointer to the first statement in the input.
	struct statement *first = new_statement();
	// A pointer to the current statement being parsed.
	struct statement *current = first;
	// Parse the input until null char.
	while (input[i[0]])
	{
		if (STATEMENT(input, i, current))
		{
			delete_statement(first);
			first = NULL;
			break;
		}
		if (!input[i[0]])
			break;
		current->next = new_statement();
		current = current->next;
	}
	// Free memory
	free(i);
	return first;
}
