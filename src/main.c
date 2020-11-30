#include <signal.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include "built_in_cmd.h"
#include "macros.h"
#include "parse.h"
#include "statement.h"

void interactive_mode()
{
	char input[MAX_COMMAND_LENGTH];
	while (1)
	{
		// Display prompt
		printf(SGR_CYAN_FG "prompt> " SGR_RESET);
		// Read input
		fgets(input, MAX_COMMAND_LENGTH, stdin);
		// Clear input buffer
		__fpurge(stdin);
		// Add input to history
		history_add(strdup(input));
		// Parse input
		struct statement *statement = parse(input);
		// Execute statement
		execute_statement(statement);
		// Delete statement
		delete_statement(statement);
	}
}

void batch_mode(char *file_path)
{
	char input[MAX_COMMAND_LENGTH];
	// Open batch file
	FILE *file = fopen(file_path, "r");
	if (file == NULL)
	{
		fprintf(stderr, SGR_RED_FG "%s: The batch file does not exist or cannot be opened.\n" SGR_RESET, SHELL_NAME);
		return;
	}
	// Read input
	for (input[MAX_COMMAND_LENGTH - 2] = '\n';
		 fgets(input, MAX_COMMAND_LENGTH, file);
		 input[MAX_COMMAND_LENGTH - 2] = '\n')
	{
		// Clear line
		if (input[MAX_COMMAND_LENGTH - 2] != '\n' && input[MAX_COMMAND_LENGTH - 3] != '\n')
			while (fgetc(file) != '\n');
		// Add input to history
		history_add(strdup(input));
		// Parse input
		struct statement *statement = parse(input);
		// Execute statement
		execute_statement(statement);
		// Delete statement
		delete_statement(statement);
	}
}

int main(int argc, char **argv)
{
	// Ignore signals
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	// Determine which mode to use
	switch (argc - 1)
	{
		case 0:
			compile_regex();
			init_data_structures();
			interactive_mode();
			break;
		case 1:
			compile_regex();
			init_data_structures();
			batch_mode(argv[1]);
			break;
		default:
			fprintf(stderr,
					SGR_RED_FG "%s: Incorrect number of arguments. Pass 0 arguments for interactive mode or 1 argument for batch mode.\n" SGR_RESET,
					SHELL_NAME);
			break;
	}
	return 0;
}
