#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "built_in_cmd.h"
#include "parse.h"

void cd(struct command *cmd)
{
	printf("This is the cd command.\n");
	
	if(parse != NULL)
		chdir(parse(*input));
	else
		chdir(getenv("HOME"));
	return;
}
