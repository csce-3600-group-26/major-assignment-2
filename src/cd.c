#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"
#include "built_in_cmd.h"
#include "parse.h"

void cd(struct command *cmd)
{
	//printf("This is the cd command.\n");
	
	char *pth = (cmd->num_args == 1) ? getenv("HOME") : cmd->args[1];
	char path[MAX_COMMAND_LENGTH];
	strcpy(path,pth);
	char cwd[MAX_COMMAND_LENGTH];
	getcwd(cwd,sizeof(cwd));
	strcat(cwd,"/");
	strcat(cwd,path);
	printf("Current directory: %s\n", path);
	int ret = chdir(path);
	if(ret != 0)
		printf("Error: Could not change to the directory \n");
	else
		system("pwd");
	return;
}
