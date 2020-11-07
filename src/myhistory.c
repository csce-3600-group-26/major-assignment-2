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
	    for(int i = history_size - 1; i >=0; i--)
	    {
	        printf("%s\n",history[i]);
	    }
	}
	//if num args =2 and arg2 = -c clear history
	else if((cmd->num_args == 2) && !(strcmp(cmd->args[1],"-c")))
	{
	    for(int i = history_size - 1; i >= 0; i--)
	    {
	        free(history[i]);
	        history[i] = NULL;
	    }
	    history_size = 0;
	}
	//if num = 3 arg2 = -e, and arg3 is a number 1-20 parse history[arg3-1] and pass to execute statement
	else if((cmd->num_args == 3) && !(strcmp(cmd->args[1],"-e")))
	{
	    int num = stoi(cmd->args[2]);
	    struct *statement stmt = parse(history[num]);
	    execute_statement(stmt);
	    delete_statement(stmt);
	}
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
