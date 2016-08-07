#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <stdio.h>      // fgets(), printf()...
#include <stdlib.h>     // malloc(), free()...
#include <unistd.h>     // chdir()

#include <string.h>     // strtok(), strcpy(), strcmp()...
#include <sys/stat.h>   // stat()
#include <stdbool.h>    // bool
//#include <ctype.h>
#include <sys/types.h>  // pid_t

#include <sys/wait.h>   // waitpid(),
//#include <time.h>
#include <assert.h>     // assert()

#ifndef NULL
#define NULL 0
#endif

// DEBUG PRINT MACRO
#ifdef DEBUG
#define dbgPrint(fmt, args...) printf(fmt, ## args)
#else
#define dbgPrint(fmt, args...)
#endif

// ERROR-MESSAGE + EXIT MACRO
#define errExit(fmt, args...) {fprintf(stderr, fmt, ## args); exit(EXIT_FAILURE);}
#define errPrint(fmt, args...) fprintf(stderr, fmt, ## args)

typedef enum {  BI_NONE, BI_CHDIR, BI_EXIT }         builtinCmd;

typedef enum {  STATUS_SUCCESS=0, STATUS_FAILURE,
                STATUS_OK,
                STATUS_EXIT, STATUS_UNRECOGNIZED }   returnStatus;

#endif // __DEFINITIONS_H__
