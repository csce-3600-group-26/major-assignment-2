#include <stdio.h>
#include <stdio_ext.h>
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
		printf("prompt> ");
		// Read input
		fgets(input, MAX_COMMAND_LENGTH, stdin);
		// Clear input buffer
		__fpurge(stdin);
		// Parse input
		struct statement *statement = parse(input);
		execute_statement(statement);
	}
}

void batch_mode(char *file_path)
{
	char input[MAX_COMMAND_LENGTH];
	// Open batch file
	FILE *file = fopen(file_path, "r");
	if (file == NULL)
	{
		fprintf(stderr, "%s: The batch file does not exist or cannot be opened.\n", SHELL_NAME);
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
		// Parse input
		struct statement *statement = parse(input);
		execute_statement(statement);
	}
}

int main(int argc, char **argv)
{
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
					"%s: Incorrect number of arguments. Pass 0 arguments for interactive mode or 1 argument for batch mode.\n",
					SHELL_NAME);
			break;
	}
	return 0;
}
