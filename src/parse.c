#include <regex.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "macros.h"
#include "parse.h"
#include "string_util.h"

// Declarations

static regex_t whitespace, built_in, alias_cmd, alias, external, argument, redirection, file_path, pipe, end;

static void STATEMENT(char *, size_t *, struct statement *);

static void COMMAND(char *, size_t *, struct statement *);

static void ARGS(char *, size_t *, struct command *);

static void ALIAS_ARGS(char *, size_t *, struct command *);

static void EXT_ARGS(char *, size_t *, struct command *);

static void PIPE(char *, size_t *, struct command *);

// Definitions

static void STATEMENT(char *input, size_t *i, struct statement *statement)
{
	regmatch_t match;
	if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		i[0] += match.rm_eo;
	if (!regexec(&end, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		return;
	}
	else COMMAND(input, i, statement);
	if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		i[0] += match.rm_eo;
	if (!regexec(&end, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		return;
	}
	else
	{
		printf("[Syntax Error] Expecting semicolon or newline.\n");
		i[0] = MAX_COMMAND_LENGTH - 1;
	}
}

static void COMMAND(char *input, size_t *i, struct statement *statement)
{
	char *command_name;
	regmatch_t match;
	if (!regexec(&built_in, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		command_name = substring(input, i[0], i[0] + match.rm_eo);
		statement->first = new_command();
		statement->first->name = command_name;
		add_arg(statement->first, command_name);
		i[0] += match.rm_eo;
		ARGS(input, i, statement->first);
	}
	else if (!regexec(&alias_cmd, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		command_name = substring(input, i[0], i[0] + match.rm_eo);
		statement->first = new_command();
		statement->first->name = command_name;
		add_arg(statement->first, command_name);
		i[0] += match.rm_eo;
		ALIAS_ARGS(input, i, statement->first);
	}
	else if (!regexec(&external, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		command_name = substring(input, i[0], i[0] + match.rm_eo);
		statement->first = new_command();
		statement->first->name = command_name;
		add_arg(statement->first, command_name);
		i[0] += match.rm_eo;
		EXT_ARGS(input, i, statement->first);
	}
	else
	{
		printf("[Syntax Error] Expecting command.\n");
		i[0] = MAX_COMMAND_LENGTH - 1;
	}
}

static void ARGS(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&argument, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			add_arg(cmd, substring(input, i[0], i[0] + match.rm_eo));
			i[0] += match.rm_eo;
			ARGS(input, i, cmd);
		}
		else
		{
			printf("[Syntax Error] Expecting argument for built-in command.\n");
			i[0] = MAX_COMMAND_LENGTH - 1;
		}
	}
}

static void ALIAS_ARGS(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&alias, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			add_arg(cmd, substring(input, i[0], i[0] + match.rm_eo));
			i[0] += match.rm_eo;
		}
		else ARGS(input, i, cmd);
	}
}

static void EXT_ARGS(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&argument, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			add_arg(cmd, substring(input, i[0], i[0] + match.rm_eo));
			i[0] += match.rm_eo;
			EXT_ARGS(input, i, cmd);
		}
		else if (!regexec(&redirection, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			char redirection_symbol = input[i[0]];
			i[0] += match.rm_eo;
			if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
			{
				i[0] += match.rm_eo;
				if (!regexec(&file_path, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
				{
					if (redirection_symbol == '<')
						cmd->input = substring(input, i[0], i[0] + match.rm_eo);
					else
						cmd->output = substring(input, i[0], i[0] + match.rm_eo);
					i[0] += match.rm_eo;
					EXT_ARGS(input, i, cmd);
				}
				else
				{
					printf("[Syntax Error] Expecting file for redirection.\n");
					i[0] = MAX_COMMAND_LENGTH - 1;
				}
			}
			else
			{
				printf("[Syntax Error] Expecting whitespace.\n");
				i[0] = MAX_COMMAND_LENGTH - 1;
			}
		}
		else PIPE(input, i, cmd);
	}
}

static void PIPE(char *input, size_t *i, struct command *cmd)
{
	regmatch_t match;
	if (!regexec(&pipe, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
	{
		i[0] += match.rm_eo;
		if (!regexec(&whitespace, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
		{
			i[0] += match.rm_eo;
			if (!regexec(&external, &input[i[0]], 1, &match, 0) && match.rm_so == 0)
			{
				cmd->pipe = new_command();
				cmd->pipe->name = substring(input, i[0], i[0] + match.rm_eo);
				i[0] += match.rm_eo;
				EXT_ARGS(input, i, cmd->pipe);
			}
			else
			{
				printf("[Syntax Error] Expecting external command for pipe.\n");
				i[0] = MAX_COMMAND_LENGTH - 1;
			}
		}
		else
		{
			printf("[Syntax Error] Expecting whitespace.\n");
			i[0] = MAX_COMMAND_LENGTH - 1;
		}
	}
}

void compile_regex()
{
	regcomp(&whitespace, "[\t ]+", REG_EXTENDED);
	regcomp(&built_in, "cd|exit|path|myhistory", REG_EXTENDED);
	regcomp(&alias_cmd, "alias", REG_EXTENDED);
	regcomp(&alias, "[A-Za-z_][A-Za-z0-9_]*='.*'", REG_EXTENDED);
	regcomp(&external,
			"^((~|\\.|\\.\\.)\\/?|\\/)?[A-Za-z0-9_][\\.A-Za-z0-9_-]*(\\/[A-Za-z0-9_][\\.A-Za-z0-9_-]*)*",
			REG_EXTENDED);
	regcomp(&argument, "[^\t ;\n<>|'\"\\]+", REG_EXTENDED);
	regcomp(&redirection, "[<>]", REG_EXTENDED);
	regcomp(&file_path,
			"^((~|\\.|\\.\\.)\\/?|\\/)?[A-Za-z0-9_][\\.A-Za-z0-9_-]*(\\/[A-Za-z0-9_][\\.A-Za-z0-9_-]*)*",
			REG_EXTENDED);
	regcomp(&pipe, "[|]", REG_EXTENDED);
	regcomp(&end, "[;\n]|(^$)|(;[\t ]*\n)", REG_EXTENDED);
}

void free_regex()
{
	regfree(&whitespace);
	regfree(&built_in);
	regfree(&alias_cmd);
	regfree(&alias);
	regfree(&external);
	regfree(&argument);
	regfree(&redirection);
	regfree(&file_path);
	regfree(&pipe);
	regfree(&end);
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
		STATEMENT(input, i, current);
		if (!input[i[0]])
			break;
		current->next = new_statement();
		current = current->next;
	}
	print_statement(first, 1);
	// Free memory
	free(i);
	return first;
}
