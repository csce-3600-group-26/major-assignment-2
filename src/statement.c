#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "built_in_cmd.h"
#include "command.h"
#include "statement.h"

struct statement *new_statement()
{
	struct statement *object = malloc(sizeof(struct statement));
	object->first = NULL;
	object->next = NULL;
	return object;
}

void delete_statement(struct statement *object)
{
	if (object)
	{
		delete_command(object->first);
		delete_statement(object->next);
		free(object);
	}
}

void print_statement(struct statement *object, int statement_number)
{
	if (object)
	{
		printf("[Statement %d]\n", statement_number);
		if (object->first)
			print_command(object->first, 0);
		print_statement(object->next, statement_number + 1);
	}
}

void execute_statement(struct statement *object)
{
	if (object)
	{
		alias_expand(object);
		if (object->first)
			execute_command(object->first);
		execute_statement(object->next);
	}
}
