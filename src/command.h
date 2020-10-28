#ifndef MAJOR_ASSIGNMENT_2_COMMAND_H
#define MAJOR_ASSIGNMENT_2_COMMAND_H

#include <stddef.h>

struct command
{
	// The name of the command.
	char *name;
	// A pointer to an array of command-line arguments.
	char **args;
	// The number of command-line arguments.
	size_t num_args;
	// The file path for redirecting input.
	char *input;
	// The file path for redirecting output.
	char *output;
	// The command that will read from this command's output.
	struct command *pipe;
};

// Instantiates a command object.
struct command *new_command();

// Frees the memory associated with a command object and its members.
void delete_command(struct command *object);

// Prints a command object in a human-readable format.
void print_command(struct command *object, int spaces);

// Executes a command.
void execute_command(struct command *object);

// Adds a command-line argument to args.
void add_arg(struct command *object, char *arg);

#endif //MAJOR_ASSIGNMENT_2_COMMAND_H
