#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "string_util.h"

char *substring(char *source, size_t start, size_t end)
{
	size_t size = end - start + 1;
	char *destination = malloc(sizeof(char) * size);
	memcpy(destination, &source[start], sizeof(char) * (size - 1));
	destination[size - 1] = '\0';
	return destination;
}
