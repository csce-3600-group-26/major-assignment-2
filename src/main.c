#include <signal.h>
#include <stdio.h>
#include "built_in_cmd.h"
#include "macros.h"
#include "mode.h"
#include "parse.h"

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
