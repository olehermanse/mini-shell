#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <stdio.h>      // fgets(), printf()...
#include <stdlib.h>     // malloc(), free()...
#include <unistd.h>     // chdir()

#include <string.h>     // strtok(), strcpy(), strcmp()...
//#include <ctype.h>
//#include <sys/types.h>

//#include <sys/wait.h>
//#include <time.h>
#include <assert.h>     // assert()

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef BOOL
#define BOOL char
#endif


typedef enum {  BI_NONE, BI_CHDIR, BI_EXIT }         builtinCmd;

typedef enum {  STATUS_SUCCESS=0, STATUS_FAILURE,
                STATUS_EXIT, STATUS_UNRECOGNIZED }   returnStatus;

#endif // __DEFINITIONS_H__
