#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in_cmd.h"
#include "string_util.h"

void alias(struct command *cmd)
{
	printf("This is the alias command.\n");
	print_command(cmd, 0);
	if (cmd->num_args == 1)
	{
		alias_print();
		return;
	}
	else if (cmd->num_args == 2)
	{
		if (!strcmp(cmd->args[1], "-c"))
		{
			alias_clear();
			return;
		}
		else
		{
			struct alias *alias = new_alias();
			size_t index_of_assignment_operator = strchr(cmd->args[1], '=') - cmd->args[1];
			alias->name = substring(cmd->args[1], 0, index_of_assignment_operator);
			alias->command = substring(cmd->args[1], index_of_assignment_operator + 2, strlen(cmd->args[1]) - 1);
			alias_add(alias);
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
	fprintf(stderr, "alias: Invalid arguments.\n");
}

void alias_add(struct alias *alias)
{
	for (size_t i = 0; aliases[i]; i++)
		if (!strcmp(aliases[i]->name, alias->name))
		{
			free(aliases[i]->command);
			aliases[i]->command = alias->command;
			free(alias->name);
			free(alias);
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
		fprintf(stderr, "alias: The alias cannot be removed because it does not exist.\n");
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
	printf("[%lu alias%s]\n", aliases_size, aliases_size == 1 ? "" : "es");
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
