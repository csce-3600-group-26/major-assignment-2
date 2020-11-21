#include <stdio.h>
#include "built_in_cmd.h"
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


void path(struct command *cmd)
{
	//printf("This is the path command.\n");
	if (cmd->num_args == 1)
	{
	    printf("PATH : %s\n", gentenv("PATH"));
	}
}
