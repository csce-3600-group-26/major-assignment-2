#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "statement.h"

struct statement *new_statement()
{
	struct statement *object = malloc(sizeof(struct statement));
	object->first = NULL;
	object->next = NULL;
	return object;
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
