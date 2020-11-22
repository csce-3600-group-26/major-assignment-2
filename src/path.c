#include "built_in_cmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "string_util.h"

#define MAX_LEN 1024;

void path(struct command *cmd)
{
	//char cmdPath[MAX_LEN];
	//char PATH[MAX_LEN];
	
	

	if (cmd->num_args == 1)
	{	
		printf("PATH : %s\n", getenv("PATH"));
	}
	else if (cmd->num_args == 3)
	{
		if (strcmp(cmd->args[1],"+") == 0)
		{
			char *path_1;
			size_t path1_size = strlen(getenv("PATH")) + 1 + strlen(cmd ->args[2]) + 1;
			path_1 = malloc(path1_size);
			strcpy(path_1,getenv("PATH"));
			strcat(path_1, ":");
			strcat(path_1, cmd->args[2]);
			
			//path_1 = strcat(getenv("PATH"), ":");
			//path_1 = strcat(path_1,cmd->args[2]);
			
			setenv ("PATH",path_1, 1);
		}
		else if (strcmp(cmd->args[1], "-") ==0) 
		{
			char *path_2;
			path_2 = getenv("PATH");
			char *tmp = strstr(path_2,cmd->args[2]);
		       
			if (tmp == NULL)
		       {
				printf("no file found\n");
			}
		       else
		       {	
			       char *path_3, *newstr, *com_string;
				path_3 = substring(path_2,0,tmp - path_2);
				newstr = substring(tmp,strlen(cmd->args[2]),strlen(tmp));
				
				size_t str_size = strlen(path_3)+ strlen(newstr) + 1;
			       com_string = malloc( str_size );	
				strcpy(com_string, path_3);
				strcat(com_string, newstr);
				setenv("PATH",com_string,1); 
			
		       }

		
		}
	}
	 






	/*
	
	int execCmd (char *cmd, int argc, char **argv)
	
		char PATH[MAX_LEN];
		char cmdPath[MAX_LEN];
		pid_t child_pid;
		int retStat;

		printf ("cmd: %s\n", cmd);
		printf ("PATH: %s\n", PATH);

		if (cmd[0] == '/')
			strcpy(cmdPath, cmd);
		else if (cmd[0] == '.')
		{
			if (realpath(cmd, cmdPath) == NULL)
			{
				fprintf(stderr, " '%s' not found\n", cmd);
				return 1;
			}
		} else {
			strcpy(cmdPath, PATH);
			strcat(cmdPath, "/");
			strcat(cmdPath, cmd);
		}
		printf("cmdPath: %s\n", cmdPath);

		if (access(cmdPath, F_OK) != 0)
		{
			fprintf(stderr, " '%s' not found\n", cmd);
			return 1;
		}
		else {
			child_pid = fork();
			if (child_pid < 0)
			{
				perror("fork");
				return -1;
			}
			else if (child_pid == 0)
			{
				if (execl(cmdPath,"","", (char *)0) == -1)
				{
					perror("execv");
					return 1;
				}
			} else {
				wait(&retStat);
				return retStat;
			}
		}
		return 1;
	*/
}
