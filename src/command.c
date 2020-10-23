#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

struct command *new_command()
{
	struct command *object = malloc(sizeof(struct command));
	object->name = NULL;
	object->args = NULL;
	object->num_args = 0;
	object->input = NULL;
	object->output = NULL;
	object->pipe = NULL;
	return object;
}

void print_command(struct command *object, int spaces)
{
	if (object->name)
		printf("%*sCommand: %s\n", spaces, "", object->name);
	if (object->args)
	{
		printf("%*sArguments:", spaces, "");
		for (size_t i = 0; i < object->num_args; i++)
			printf(" %s", object->args[i]);
		printf("\n");
	}
	if (object->input)
		printf("%*sInput: %s\n", spaces, "", object->input);
	if (object->output)
		printf("%*sOutput: %s\n", spaces, "", object->output);
	if (object->pipe)
	{
		printf("%*sPipe:\n", spaces, "");
		print_command(object->pipe, spaces + 1);
	}
}

void add_arg(struct command *object, char *arg)
{
	object->num_args++;
	if (!object->args)
	{
		object->args = malloc(sizeof(char *));
		object->args[0] = arg;
		return;
	}
	char **new_args = malloc(sizeof(char *) * object->num_args);
	new_args[object->num_args - 1] = arg;
	memcpy(new_args, object->args, sizeof(char *) * (object->num_args - 1));
	free(object->args);
	object->args = new_args;
}
