#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in_cmd.h"
#include "macros.h"
#include "parse.h"
#include "statement.h"

void myhistory(struct command *cmd)
{
	//if num args =1 print history
	if(cmd->num_args == 1)
	{
	    for(int i = history_size - 1; i >=1; i--) //for each entry in history...
	    {
	        printf("%d: %s\n", i,history[i]); //... print the entry at history[i]
	    }
	}
	//if num args =2 and arg2 = -c clear history
	else if((cmd->num_args == 2) && !(strcmp(cmd->args[1],"-c")))
	{
	    for(int i = history_size - 1; i >= 1; i--) //for each entry in history...
	    {
	        free(history[i]); //clear the allocated memory
	        history[i] = NULL; //re initialize that entry to NULL
	    }
	    history_size = 0; //set the number of entries to 0
	}
	//if num = 3 arg2 = -e, and arg3 is a number 1-20 parse history[arg3-1] and pass to execute statement
	else if((cmd->num_args == 3) && !(strcmp(cmd->args[1],"-e")))
	{
	    int num = atoi(cmd->args[2])+1;
	    if((num < 1) || (num >= history_size)) //check if the entered history to execute is within the proper range
	    {
			fprintf(stderr, SGR_RED_FG "input is outside of acceptable range\n" SGR_RESET);
	    }
	    else
		{
			struct statement* stmt = parse(history[num]); //parse history[i] into an executable statement
			if (!stmt)
				return;
			if(stmt->first->num_args >=3 && stmt->first->args[2] != NULL) //if the -e command is another -e command
			{
				if(atoi(stmt->first->args[2]) == num-1) // if the end of the history stmt has the same command to execute as it's place in history
					fprintf(stderr, SGR_RED_FG "Error: that would cause a Segmentation Fault\n" SGR_RESET); // print an error to prevent an infinite loop.
			}
			else //if there are no errors that would happen, execute the statement
			{
				execute_statement(stmt);
			}
			delete_statement(stmt);
		}
	}
}

void history_add(char *entry)
{
	char *new_history[21];
	new_history[0] = entry;
	memcpy(&new_history[1], history, sizeof(char *) * 20);
	free(history[20]);
	memcpy(history, new_history, sizeof(char *) * 21);
	history_size += history_size < 21 ? 1 : 0;
}
