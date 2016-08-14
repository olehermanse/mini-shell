#ifndef __EXTERNAL_COMMANDS_H__
#define __EXTERNAL_COMMANDS_H__

#include "definitions.h"

// Checks results from fork() and waitpid() system calls
// Prints error message if needed and sets appropriate status
// Return: int returnStatus (See enum in definitions.h)
en_status parentResult(pid_t pid1, pid_t pid2, int status);

// Fork and execve, don't wait, exit. Used to get rid of zombie processes.
// Parent waits for this process. Resulting in orphan child process.
en_status secondFork(char* path, char** argv, int argc, char* envp[], int* fds);

// Ampersand & - Fork and execve without waiting for child process to finish
// Return: int returnStatus (See enum in definitions.h)
en_status forkExecNoWait(char* path, char** argv, int argc, char* envp[], int* fds);

// Regular fork execve (no &)
// Return: int returnStatus (See enum in definitions.h)
en_status forkExecWait(char* path, char** argv, int argc, char *envp[], int* fds);

// Check if there is an executable at path
// Return: bool true if executable
bool checkExecutable(char* path);

// Searches working working directory and directories in PATH for executable
// Return: STATUS_SUCCESS if executable found, STATUS_FAILURE otherwise
en_status findExecutable(char* name, char* pathOutput);

// Searches for an executable, forks and executes
// Doesn't wait for child if & is found
// Return: returnStatus from called function(s)
en_status processExternalCmd(char** argv, int num, char* envp[], int* fds);

#endif // __EXTERNAL_COMMANDS_H__
