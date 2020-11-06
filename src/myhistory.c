#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in_cmd.h"

void myhistory(struct command *cmd)
{
	printf("This is the myhistory command.\n");
}

void history_add(char *entry)
{
	char *new_history[20];
	new_history[0] = entry;
	memcpy(&new_history[1], history, sizeof(char *) * 19);
	free(history[19]);
	memcpy(history, new_history, sizeof(char *) * 20);
	history_size += history_size < 20 ? 1 : 0;
}
