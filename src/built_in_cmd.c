#include <stdlib.h>
#include "built_in_cmd.h"

void init_data_structures()
{
	// cd
	previous_directory = NULL;
	// path

	// myhistory
	history = malloc(20*sizeof(char *));
	history[0] = NULL;
	history[1] = NULL;
	history[2] = NULL;
	history[3] = NULL;
	history[4] = NULL;
	history[5] = NULL;
	history[6] = NULL;
	history[7] = NULL;
	history[8] = NULL;
	history[9] = NULL;
	history[10] = NULL;
	history[11] = NULL;
	history[12] = NULL;
	history[13] = NULL;
	history[14] = NULL;
	history[15] = NULL;
	history[16] = NULL;
	history[17] = NULL;
	history[18] = NULL;
	history[19] = NULL;
	history_size = 0;
	// alias
	aliases = malloc(sizeof(struct alias *));
	aliases[0] = NULL;
	aliases_size = 0;
}
