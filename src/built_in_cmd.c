#include <stdlib.h>
#include "built_in_cmd.h"

void init_data_structures()
{
	// cd
	previous_directory = NULL;
	// path

	// myhistory
	history = malloc(sizeof(struct command *));
	history[0] = NULL;
	history_size = 0;
	// alias
	aliases = malloc(sizeof(struct alias *));
	aliases[0] = NULL;
	aliases_size = 0;
}
