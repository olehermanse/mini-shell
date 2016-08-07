#include "builtincommands.h"

// strcmp switch to select appropriate built in function
// Return: int builtInCmd (See enum in definitions.h)
int builtinSwitch(char* word){
    // Empty cmd - do nothing:
    if(word == NULL)
        return BI_NONE;
    if(strlen(word) == 0)
        return BI_NONE;

    // Change directory:
    if(strcmp(word, "cd") == 0)
        return BI_CHDIR;
    if(strcmp(word, "chdir") == 0)
        return BI_CHDIR;

    // Exit:
    if(strcmp(word, "exit") == 0)
        return BI_EXIT;
    if(strcmp(word, "quit") == 0)
        return BI_EXIT;
    if(strcmp(word, "exit()") == 0)
        return BI_EXIT;
    if(strcmp(word, "quit()") == 0)
        return BI_EXIT;

    // Unknown - do nothing:
    return BI_NONE;
}

// Change current working directory. This cannot be done in fork,
// as we want cd to apply to the main processes' working directory
// Return: int returnStatus (See enum in definitions.h)
int builtinChangeDir(char** words){
    int rval = chdir(words[1]);
    if(rval == 0)
        return STATUS_SUCCESS;
    errPrint("Folder not found: '%s'\n", words[1]);
    return STATUS_OK;
}

// Entry point for built in commands. Switch selects cd, exit or unrecognized
// Return: int returnStatus (See enum in definitions.h)
int processBuiltinCmd(char** words, int* fds){
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
