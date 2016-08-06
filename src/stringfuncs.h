#ifndef __STRING_FUNCS_H__
#define __STRING_FUNCS_H__

#include "definitions.h"

// Loop to free all cstrings in array of cstrings as well as array itself.
// Return: NULL pointer for assignment.
void ** freeArray(void ** pointerArray);

// Copies a null terminated array of null terminated.
// strings(cstrings) to a newly mallocd similar structure.
// Return: new copied null terminated array of cstrings.
// mallocs: all strings as well as array.
char** strcpyArray(char** strings);

// Parses a string using strtok. String is split into tokens("words")
// Return: null terminated array(pointer) of cstrings.
// mallocs: array pointer only (strings point directly to buffer)
char** splitString(char* buffer, const char* sep);

#endif // __STRING_FUNCS_H__