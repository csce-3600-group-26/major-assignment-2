#include <stdio.h>
#include <stdlib.h>
#include "built_in_cmd.h"

void alias(struct command *cmd)
{
	printf("This is the alias command.\n");
	exit(0);
}
