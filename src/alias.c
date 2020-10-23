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
	if (cmd->num_args > 0)
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
				break;
			}
			else
				i++;
		}
		alias_add(alias);
	}

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
