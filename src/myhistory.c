#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in_cmd.h"
#include "parse.h"
#include "statement.h"

void myhistory(struct command *cmd)
{
	//printf("This is the myhistory command.\n");
	//if num args =1 print history
	if(cmd->num_args == 1)
	{
	    for(int i = history_size - 1; i >=1; i--)
	    {
	        printf("%d: %s\n", i,history[i]);
	    }
	}
	//if num args =2 and arg2 = -c clear history
	else if((cmd->num_args == 2) && !(strcmp(cmd->args[1],"-c")))
	{
	    for(int i = history_size - 1; i >= 1; i--)
	    {
	        free(history[i]);
	        history[i] = NULL;
	    }
	    history_size = 0;
	}
	//if num = 3 arg2 = -e, and arg3 is a number 1-20 parse history[arg3-1] and pass to execute statement
	else if((cmd->num_args == 3) && !(strcmp(cmd->args[1],"-e")))
	{
	    int num = atoi(cmd->args[2])+1;
	    struct statement* stmt = parse(history[num]);
	    if((num < 1) || (num >= history_size))
	    {
	        printf("input is outside of acceptable range\n");
	    }
	    else if(stmt->first->num_args >=3 && stmt->first->args[2] != NULL)
	    {
	        if(atoi(stmt->first->args[2]) == num-1)
    	        printf("Error: that would cause a Segmentation Fault\n");
	    }*/
	    else
	    {
	        execute_statement(stmt);
	    }
	    delete_statement(stmt);
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
