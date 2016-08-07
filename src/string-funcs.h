#ifndef __STRING_FUNCS_H__
#define __STRING_FUNCS_H__

#include "definitions.h"

// Loops through null terminated string and replaces all occurences
// of "match" with "replace"
// Return: no. of characters replaced
int strReplace(char* str, char match, char replace);

// Loop to free all cstrings in array of cstrings as well as array itself.
// Return: NULL pointer for assignment.
void ** freeArray(void ** pointerArray);

// Copies a null terminated array of null terminated.
// strings(cstrings) to a newly mallocd similar structure.
// Return: new copied null terminated array of cstrings.
// mallocs: all strings as well as array.
char** strcpyArray(char** strings);

// Counts words in null terminated array(/pointer)
// Return: count of words(excluding null terminator)
int wordCount(char** words);

// Parses a string using strtok. String is split into tokens("words")
// Return: null terminated array(pointer) of cstrings.
// mallocs: array pointer only (strings point directly to buffer)
char** splitString(char* buffer, const char* sep, const char* substrsyms);

#endif // __STRING_FUNCS_H__
