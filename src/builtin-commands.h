#ifndef __BUILTIN_COMMANDS_H__
#define __BUILTIN_COMMANDS_H__

#include "definitions.h"

// strcmp switch to select appropriate built in function
// Return: int builtInCmd (See enum in definitions.h)
en_builtin builtinSwitch(char* word);

// Change current working directory. This cannot be done in fork,
// as we want cd to apply to the main processes' working directory
// Return: int returnStatus (See enum in definitions.h)
en_status builtinChangeDir(char** words);

// Entry point for built in commands. Switch selects cd, exit or unrecognized
// Return: int returnStatus (See enum in definitions.h)
en_status processBuiltinCmd(char** words, int* fds);

#endif // __BUILTIN_COMMANDS_H__
