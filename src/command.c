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

// The number of commands in the pipeline.
static int pipeline_size = 0;

// Executes an external command.
static void execute_external_command(struct command *object)
{
	// The read end fd[0] and write end fd[1] of the pipe between the current command and the next command.
	int fd[2];
	pipe(fd);
	pipeline_size++;
	// The process ID of the child process.
	pid_t child = fork();
	if (!child)
	{
		if (object->input)
		{
			int fd_input = open(object->input, O_RDONLY, 0);
			dup2(fd_input, STDIN_FILENO);
			close(fd_input);
		}
		else if (fd_read_end != -1)
		{
			// Input Pipelining
			dup2(fd_read_end, 0);
		}
		if (object->output)
		{
			// Output Redirection
			fd[1] = open(output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			object->output = 1; 
		}
		else if (object->pipe)
		{
			// Output Pipelining
			dup2(fd[1],1);
		}
		// Close the read end and write end in the child process.
		if (fd_read_end != -1)
			close(fd_read_end);
		close(fd[0]);
		close(fd[1]);
		execvp(object->name, object->args);
		fprintf(stderr, SGR_RED_FG "%s: %s.\n" SGR_RESET, SHELL_NAME, strerror(errno));
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
		execute_external_command(object->pipe);
		return;
	}
	if (fd_read_end != -1)
		close(fd_read_end);
	for (int i = 0; i < pipeline_size; i++)
		waitpid(-1, NULL, WUNTRACED);
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
		execute_external_command(object);
		exit(0);
	}
	waitpid(child, NULL, WUNTRACED);
	tcsetpgrp(STDIN_FILENO, getpgid(getpid()));
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
