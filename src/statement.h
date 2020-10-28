#ifndef MAJOR_ASSIGNMENT_2_STATEMENT_H
#define MAJOR_ASSIGNMENT_2_STATEMENT_H

#include "command.h"

struct statement
{
	// The first command to execute in the statement.
	struct command *first;
	// The next statement to run after all commands in this statement have been executed.
	struct statement *next;
};

// Instantiates a statement object.
struct statement *new_statement();

// Frees the memory associated with a statement object and its members.
void delete_statement(struct statement *object);

// Prints a statement object in a human-readable format.
void print_statement(struct statement *object, int statement_number);

// Executes all commands in a statement and in succeeding statements.
void execute_statement(struct statement *object);

#endif //MAJOR_ASSIGNMENT_2_STATEMENT_H
