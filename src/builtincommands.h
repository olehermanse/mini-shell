#ifndef __BUILTIN_COMMANDS_H__
#define __BUILTIN_COMMANDS_H__

#include "definitions.h"

int builtinSwitch(char* word){
    if(word == NULL)
        return BI_NONE;
    if(strlen(word) == 0)
        return BI_NONE;

    if(strcmp(word, "cd") == 0)
        return BI_CHDIR;
    if(strcmp(word, "chdir") == 0)
        return BI_CHDIR;

    if(strcmp(word, "exit") == 0)
        return BI_EXIT;
    if(strcmp(word, "quit") == 0)
        return BI_EXIT;
    if(strcmp(word, "exit()") == 0)
        return BI_EXIT;
    if(strcmp(word, "quit()") == 0)
        return BI_EXIT;

    return BI_NONE;
}

int builtinChangeDir(char** words){
    int rval = chdir(words[1]);
    if(rval == 0)
        return STATUS_SUCCESS;
    printf("Folder not found: '%s'\n", words[1]);
    return STATUS_FAILURE;
}

int processBuiltinCmd(char** words){
    switch (builtinSwitch(words[0])){
        case BI_CHDIR:
            return builtinChangeDir(words);
        case BI_EXIT:
            return STATUS_EXIT;
        case BI_NONE:
        default:
            return STATUS_UNRECOGNIZED;
    }
}

#endif // __BUILTIN_COMMANDS_H__
