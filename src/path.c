#include "built_in_cmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "string_util.h"

void path(struct command *cmd)
{
	

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
	 
}
