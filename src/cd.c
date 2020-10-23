#include <stdio.h>
#include <stdlib.h>
#include "built_in_cmd.h"

void cd(struct command *cmd)
{
	printf("This is the cd command.\n");
	exit(0);
}
