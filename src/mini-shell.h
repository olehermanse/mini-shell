#ifndef __MINI_SHELL__
#define __MINI_SHELL__

#include "stringfuncs.h"
#include "builtincommands.h"
#include "externalcommands.h"

// Processes a query by splitting and attempting to run built in or
// external command.
// Return: int returnStatus from called function(s) (See enum in definitions.h)
int processCmd(char* buffer, char* envp[], int* fds);

#endif // __MINI_SHELL__
