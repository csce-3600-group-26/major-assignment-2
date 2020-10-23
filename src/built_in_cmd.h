#ifndef MAJOR_ASSIGNMENT_2_BUILT_IN_CMD_H
#define MAJOR_ASSIGNMENT_2_BUILT_IN_CMD_H

#include "command.h"

// Changes the current working directory.
void cd(struct command *cmd);

// Shows or modifies the system path.
void path(struct command *cmd);

// Lists or executes previous commands.
void myhistory(struct command *cmd);

// Creates shortcuts for commands.
void alias(struct command *cmd);

#endif //MAJOR_ASSIGNMENT_2_BUILT_IN_CMD_H
