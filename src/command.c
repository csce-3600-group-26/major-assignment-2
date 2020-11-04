#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "built_in_cmd.h"
#include "command.h"
#include "macros.h"
#include "parse.h"

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

// Returns true if previous_aliases contains the specified alias.
static int previous_aliases_contains(char *alias)
{
	if (previous_aliases)
		for (size_t i = 0; previous_aliases[i]; i++)
			if (!strcmp(alias, previous_aliases[i]))
				return 1;
	return 0;
}

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

void delete_command(struct command *object)
{
	if (object)
	{
		free(object->name);
		for (size_t i = 0; object->args[i]; i++)
			free(object->args[i]);
		free(object->args);
		free(object->input);
		free(object->output);
		delete_command(object->pipe);
		free(object);
	}
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

// The read end of the pipe between the previous command and the current command.
static int fd_read_end = -1;

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
	if (!previous_aliases_contains(object->name))
		for (size_t i = 0; aliases[i]; i++)
			if (!strcmp(object->name, aliases[i]->name))
			{
				// Parse the alias.
				struct statement *alias_statement = parse(aliases[i]->command);
				// Return if the alias's statement is invalid.
				if (!alias_statement)
					return;
				// Keep track of aliases that have already been expanded.
				previous_aliases_add(aliases[i]->name);
				// Get the last statement.
				struct statement *last_statement = alias_statement;
				while (last_statement->next)
					last_statement = last_statement->next;
				// Get the last command.
				struct command *last_command = NULL;
				if (last_statement->first)
				{
					last_command = last_statement->first;
					while (last_command->pipe)
						last_command = last_command->pipe;
				}
				else if (object->num_args >= 2)
				{
					last_statement->first = new_command();
					last_command = last_statement->first;
					last_command->name = strdup(object->args[1]);
				}
				// Copy all arguments from the current command to the alias's last command.
				if (last_command)
				{
					for (size_t j = 1; j < object->num_args; j++)
						add_arg(last_command, strdup(object->args[j]));
					if (object->input)
					{
						free(last_command->input);
						last_command->input = strdup(object->input);
					}
					if (object->output)
					{
						free(last_command->output);
						last_command->output = strdup(object->output);
					}
					if (object->pipe)
						last_command->pipe = object->pipe;
				}
				execute_statement(alias_statement);
				if (last_command)
					last_command->pipe = NULL;
				delete_statement(alias_statement);
				previous_aliases_clear();
				return;
			}
	// The read end fd[0] and write end fd[1] of the pipe between the current command and the next command.
	int fd[2];
	pipe(fd);
	// The process ID of the child process.
	pid_t child = fork();
	if (!child)
	{
		setpgid(getpid(), 0);
		tcsetpgrp(STDIN_FILENO, getpgid(getpid()));
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		if (object->input)
		{
			// Input Redirection
		}
		else if (fd_read_end != -1)
		{
			// Input Pipelining
		}
		if (object->output)
		{
			// Output Redirection
		}
		else if (object->pipe)
		{
			// Output Pipelining
		}
		// Close the read end and write end in the child process.
		if (fd_read_end != -1)
			close(fd_read_end);
		close(fd[0]);
		close(fd[1]);
		execvp(object->name, object->args);
		fprintf(stderr, "%s: %s.\n", SHELL_NAME, strerror(errno));
		exit(0);
	}
	// Close the read end and write end in the parent process.
	if (fd_read_end != -1)
		close(fd_read_end);
	close(fd[1]);
	// Store the read end for the next command to use.
	fd_read_end = fd[0];
	if (object->pipe)
	{
		execute_command(object->pipe);
		return;
	}
	if (fd_read_end != -1)
		close(fd_read_end);
	waitid(P_PID, child, NULL, WEXITED | WSTOPPED);
	tcsetpgrp(STDIN_FILENO, getpgid(getpid()));
	// Reset the read end.
	fd_read_end = -1;
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
