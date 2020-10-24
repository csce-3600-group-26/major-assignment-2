#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "built_in_cmd.h"
#include "command.h"
#include "macros.h"
#include "parse.h"

struct command *new_command()
{
	struct command *object = malloc(sizeof(struct command));
	object->name = NULL;
	object->args = malloc(sizeof(char *));
	object->args[0] = NULL;
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

void execute_command(struct command *object)
{
	if (!strcmp(object->name, "exit"))
		exit(0);
	if (!strcmp(object->name, "cd"))
	{
		cd(object);
		return;
	}
	else if (!strcmp(object->name, "path"))
	{
		path(object);
		return;
	}
	else if (!strcmp(object->name, "myhistory"))
	{
		myhistory(object);
		return;
	}
	else if (!strcmp(object->name, "alias"))
	{
		alias(object);
		return;
	}
	// Find a matching alias and execute it.
	for (size_t i = 0; aliases[i]; i++)
		if (!strcmp(object->name, aliases[i]->name))
		{
			execute_statement(parse(aliases[i]->command));
			return;
		}
	// The process ID of the child process.
	pid_t child = fork();
	if (!child)
	{
		execvp(object->name, object->args);
		fprintf(stderr, "%s: %s.\n", SHELL_NAME, strerror(errno));
		exit(0);
	}
	waitpid(child, NULL, 0);
}

void add_arg(struct command *object, char *arg)
{
	object->num_args++;
	char **new_args = malloc(sizeof(char *) * (object->num_args + 1));
	new_args[object->num_args] = NULL;
	new_args[object->num_args - 1] = arg;
	memcpy(new_args, object->args, sizeof(char *) * (object->num_args - 1));
	free(object->args);
	object->args = new_args;
}
