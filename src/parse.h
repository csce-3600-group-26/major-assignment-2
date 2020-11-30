#ifndef MAJOR_ASSIGNMENT_2_PARSE_H
#define MAJOR_ASSIGNMENT_2_PARSE_H

#include <regex.h>
#include "statement.h"

regex_t regex_whitespace, regex_built_in, regex_alias_cmd, regex_alias, regex_external, regex_argument,
		regex_redirection, regex_file_path, regex_pipe, regex_end, regex_external_negative_lookahead;

// Converts a string into a statement object. Returns NULL if the string produces a syntax error. compile_regex() must
// be called exactly 1 time in the program before this function can be used.
struct statement *parse(char *input);

// Compiles the regular expressions for the parser.
void compile_regex();

// Frees the memory associated with the regular expressions.
void free_regex();

#endif //MAJOR_ASSIGNMENT_2_PARSE_H
