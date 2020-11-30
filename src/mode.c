#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include "built_in_cmd.h"
#include "macros.h"
#include "mode.h"
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
	batch_file = fopen(file_path, "r");
	if (batch_file == NULL)
	{
		fprintf(stderr, SGR_RED_FG "%s: The batch file does not exist or cannot be opened.\n" SGR_RESET, SHELL_NAME);
		return;
	}
	// Read input
	for (input[MAX_COMMAND_LENGTH - 2] = '\n';
		 fgets(input, MAX_COMMAND_LENGTH, batch_file);
		 input[MAX_COMMAND_LENGTH - 2] = '\n')
	{
		// Clear line
		if (input[MAX_COMMAND_LENGTH - 2] != '\n' && input[MAX_COMMAND_LENGTH - 3] != '\n')
			while (fgetc(batch_file) != '\n');
		// Add input to history
		history_add(strdup(input));
		// Parse input
		struct statement *statement = parse(input);
		// Execute statement
		execute_statement(statement);
		// Delete statement
		delete_statement(statement);
	}
	// Close batch file
	fclose(batch_file);
}
