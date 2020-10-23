#ifndef MAJOR_ASSIGNMENT_2_PARSE_H
#define MAJOR_ASSIGNMENT_2_PARSE_H

#include "statement.h"

// Converts a string into a statement object. compile_regex() must be called exactly 1 time in the program before this
// function can be used.
struct statement *parse(char *input);

// Compiles the regular expressions for the parser.
void compile_regex();

// Frees the memory associated with the regular expressions.
void free_regex();

#endif //MAJOR_ASSIGNMENT_2_PARSE_H
