#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in_cmd.h"
#include "macros.h"
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
	if (cmd->num_args > 1)
	{
		if (cmd->args[1][0] == '-')
		{
			if (cmd->args[1][1])
				if (cmd->args[1][1] == 'c' && !cmd->args[1][2])
				{
					alias_clear();
					return;
				}
				else if (cmd->args[1][1] == 'r' && !cmd->args[1][2])
				{
					return;
				}
		}
		else
		{
			struct alias *alias = malloc(sizeof(struct alias));
			alias->name = NULL;
			alias->command = NULL;
			size_t i = 0, start = 0;
			while (cmd->args[1][i])
			{
				if (cmd->args[1][i] == '=')
				{
					alias->name = substring(cmd->args[1], 0, i);
					break;
				}
				else
					i++;
			}
			while (cmd->args[1][i])
			{
				if (cmd->args[1][i] == '\'')
				{
					start = ++i;
					break;
				}
				else
					i++;
			}
			while (cmd->args[1][i])
			{
				if (cmd->args[1][i] == '\'')
				{
					alias->command = substring(cmd->args[1], start, i);
					alias_add(alias);
					return;
				}
				else
					i++;
			}
		}
	}
	fprintf(stderr, "%s: alias: Invalid arguments.\n", SHELL_NAME);
}

void alias_add(struct alias *alias)
{
	aliases_size++;
	struct alias **new_aliases = malloc(sizeof(struct alias *) * (aliases_size + 1));
	new_aliases[aliases_size] = NULL;
	new_aliases[aliases_size - 1] = alias;
	memcpy(new_aliases, aliases, sizeof(struct alias *) * (aliases_size - 1));
	free(aliases);
	aliases = new_aliases;
}

void alias_clear()
{
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
