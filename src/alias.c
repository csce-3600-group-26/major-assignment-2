#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in_cmd.h"
#include "macros.h"
#include "parse.h"
#include "statement.h"
#include "string_util.h"

void alias(struct command *cmd)
{
	if (cmd->num_args == 1)
	{
		alias_print();
		return;
	}
	else if (cmd->num_args == 2)
	{
		regmatch_t match;
		if (!strcmp(cmd->args[1], "-c"))
		{
			alias_clear();
			return;
		}
		else if (!regexec(&regex_alias, cmd->args[1], 1, &match, 0) && match.rm_so == 0)
		{
			struct alias *alias = new_alias();
			size_t index_of_assignment_operator = strchr(cmd->args[1], '=') - cmd->args[1];
			alias->name = substring(cmd->args[1], 0, index_of_assignment_operator);
			alias->command = substring(cmd->args[1], index_of_assignment_operator + 2, strlen(cmd->args[1]) - 1);
			struct statement *statement = parse(alias->command);
			if (statement)
			{
				if (statement->first)
					alias_add(alias);
				else
				{
					fprintf(stderr, SGR_RED_FG "alias: The alias cannot be added because it is empty.\n" SGR_RESET);
					delete_alias(alias);
				}
			}
			else
				delete_alias(alias);
			delete_statement(statement);
			return;
		}
	}
	else if (cmd->num_args == 3)
	{
		if (!strcmp(cmd->args[1], "-r"))
		{
			alias_remove(cmd->args[2]);
			return;
		}
	}
	fprintf(stderr, SGR_RED_FG "alias: Invalid arguments.\n" SGR_RESET);
}

void alias_add(struct alias *alias)
{
	for (size_t i = 0; aliases[i]; i++)
		if (!strcmp(aliases[i]->name, alias->name))
		{
			delete_alias(aliases[i]);
			aliases[i] = alias;
			return;
		}
	aliases_size++;
	struct alias **new_aliases = malloc(sizeof(struct alias *) * (aliases_size + 1));
	new_aliases[aliases_size] = NULL;
	new_aliases[aliases_size - 1] = alias;
	memcpy(new_aliases, aliases, sizeof(struct alias *) * (aliases_size - 1));
	free(aliases);
	aliases = new_aliases;
}

void alias_remove(char *alias_name)
{
	size_t i = 0;
	for (; aliases[i]; i++)
		if (!strcmp(aliases[i]->name, alias_name))
			break;
	if (!aliases[i])
	{
		fprintf(stderr, SGR_RED_FG "alias: The alias cannot be removed because it does not exist.\n" SGR_RESET);
		return;
	}
	aliases_size--;
	struct alias **new_aliases = malloc(sizeof(struct alias *) * (aliases_size + 1));
	memcpy(new_aliases, aliases, sizeof(struct alias *) * i);
	memcpy(&new_aliases[i], &aliases[i + 1], sizeof(struct alias *) * (aliases_size - i + 1));
	delete_alias(aliases[i]);
	free(aliases);
	aliases = new_aliases;
}

void alias_clear()
{
	for (size_t i = 0; aliases[i]; i++)
		delete_alias(aliases[i]);
	free(aliases);
	aliases = malloc(sizeof(struct alias *));
	aliases[0] = NULL;
	aliases_size = 0;
}

void alias_print()
{
	printf("[%zu alias%s]\n", aliases_size, aliases_size == 1 ? "" : "es");
	for (size_t i = 0; aliases[i]; i++)
		printf("alias %s='%s'\n", aliases[i]->name, aliases[i]->command);
}

struct alias *new_alias()
{
	struct alias *alias = malloc(sizeof(struct alias));
	alias->name = NULL;
	alias->command = NULL;
	return alias;
}

void delete_alias(struct alias *object)
{
	free(object->name);
	free(object->command);
	free(object);
}

// An array of strings, each representing an alias that has already been expanded.
static char **previous_aliases = NULL;

// The number of aliases in previous_aliases.
static size_t previous_aliases_size = 0;

// Adds an alias to previous_aliases.
static void previous_aliases_add(char *alias)
{
	if (previous_aliases)
	{
		previous_aliases_size++;
		char **new_previous_aliases = malloc(sizeof(char *) * (previous_aliases_size + 1));
		new_previous_aliases[previous_aliases_size] = NULL;
		new_previous_aliases[previous_aliases_size - 1] = alias;
		memcpy(new_previous_aliases, previous_aliases, sizeof(char *) * (previous_aliases_size - 1));
		free(previous_aliases);
		previous_aliases = new_previous_aliases;
	}
	else
	{
		previous_aliases_size = 1;
		previous_aliases = malloc(sizeof(char *) * 2);
		previous_aliases[1] = NULL;
		previous_aliases[0] = alias;
	}
}

// Removes all aliases in previous_aliases.
static void previous_aliases_clear()
{
	if (previous_aliases)
	{
		previous_aliases_size = 0;
		free(previous_aliases);
		previous_aliases = NULL;
	}
}

// Returns a pointer to the matching alias. Returns NULL if the alias has already been expanded of if it is not found.
static struct alias *previous_aliases_find(char *alias)
{
	if (previous_aliases)
		for (size_t i = 0; previous_aliases[i]; i++)
			if (!strcmp(alias, previous_aliases[i]))
				return NULL;
	for (size_t i = 0; aliases[i]; i++)
		if (!strcmp(alias, aliases[i]->name))
			return aliases[i];
	return NULL;
}

void alias_expand(struct statement *statement)
{
	if (statement)
		for (struct command *current_command = statement->first, *previous_command = NULL;
			 current_command;
			 previous_command = current_command, current_command = current_command->pipe)
		{
			while (1)
			{
				struct alias *alias = previous_aliases_find(current_command->name);
				if (!alias)
					break;
				// Keep track of aliases that have already been expanded.
				previous_aliases_add(alias->name);
				// Parse the alias.
				struct statement *alias_statement = parse(alias->command);
				struct command *alias_command = alias_statement->first;
				alias_statement->first = NULL;
				delete_statement(alias_statement);
				// Get the last command.
				struct command *last_command = alias_command;
				while (last_command->pipe)
					last_command = last_command->pipe;
				// Copy all arguments from the current command to the alias's last command.
				for (size_t j = 1; j < current_command->num_args; j++)
					add_arg(last_command, strdup(current_command->args[j]));
				if (current_command->input)
				{
					free(last_command->input);
					last_command->input = strdup(current_command->input);
				}
				if (current_command->output)
				{
					free(last_command->output);
					last_command->output = strdup(current_command->output);
				}
				if (current_command->pipe)
				{
					last_command->pipe = current_command->pipe;
					current_command->pipe = NULL;
				}
				// Connect the alias's command to the previous command.
				if (previous_command)
					previous_command->pipe = alias_command;
				else
					statement->first = alias_command;
				// Set the current command to the alias's command.
				delete_command(current_command);
				current_command = alias_command;
			}
			previous_aliases_clear();
		}
}
