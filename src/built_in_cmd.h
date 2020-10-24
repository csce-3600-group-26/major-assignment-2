#ifndef MAJOR_ASSIGNMENT_2_BUILT_IN_CMD_H
#define MAJOR_ASSIGNMENT_2_BUILT_IN_CMD_H

#include <stddef.h>
#include "command.h"

/* ------------------cd------------------ */

// Changes the current working directory.
void cd(struct command *cmd);

// A string containing the path to the previous directory.
char *previous_directory;

/* -----------------path----------------- */

// Shows or modifies the system path.
void path(struct command *cmd);

/* ---------------myhistory-------------- */

// Lists or executes previous commands.
void myhistory(struct command *cmd);

// A null-terminated array of command objects.
struct command **history;

// The number of command objects in history.
size_t history_size;

// Adds a command object to history.
void history_add(struct command *cmd);

/* -----------------alias---------------- */

// Creates shortcuts for commands.
void alias(struct command *cmd);

// Data structure for storing aliases.
struct alias
{
	// The name of the alias.
	char *name;
	// The command to execute when the name is called.
	char *command;
};

// Instantiates an alias object.
struct alias *new_alias();

// Frees the memory associated with an alias object and its members.
void delete_alias(struct alias *object);

// A null-terminated array of alias objects.
struct alias **aliases;

// The number of alias objects in aliases.
size_t aliases_size;

// Adds an alias. If the alias name is already taken, that alias will be overwritten.
void alias_add(struct alias *alias);

// Removes an alias.
void alias_remove(char *alias_name);

// Removes all aliases.
void alias_clear();

// Prints all aliases.
void alias_print();

/* -------------------------------------- */

// Initializes all data structures for built-in commands.
void init_data_structures();

#endif //MAJOR_ASSIGNMENT_2_BUILT_IN_CMD_H
