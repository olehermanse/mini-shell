#include "mini-shell.h"

// Processes a query by splitting and attempting to run built in or
// external command.
// Return: int returnStatus from called function(s) (See enum in definitions.h)
int processCmd(char* buffer, char* envp[]){
    assert(buffer);

    char** argv = splitString(buffer, " \t\n\v\f\r");
    int num = wordCount(argv);
    if(num == 0){
        free(argv);
        return STATUS_SUCCESS;
    }

    int cmdResult = processBuiltinCmd(argv);
    if(cmdResult == STATUS_UNRECOGNIZED){
        cmdResult = processExternalCmd(argv, num, envp);
    }
    free((void**)(argv));
    argv = NULL;
    return cmdResult;
}

// Main function sets up permanent variables and runs a main loop
// prompting for and processing input from stdin
int main(int argc, char** argv, char *envp[]){
    dbgPrint("Debug print enabled.\n");

    // Main buffer used for input:
    const int MAX_INPUT_SIZE = 255;
    char buffer[MAX_INPUT_SIZE];

    // Get username from environment variable:
    char* user = NULL;
    strcpy(buffer,getenv("USER"));
    user = (char*)malloc(strlen(buffer)+1);
    assert(user);
    strcpy(user, buffer);

    // Return value from fgets (check for EOF/error):
    char* result = buffer;

    // Count # of commands entered (i.e. number of enter presses):
    int cmdCounter = 0;
    
    // Main program loop:
    while(result){
        // Print prompt with username and increasing command count:
        printf("%s@mini-shell %i> ", user, cmdCounter);

        // Store user input in buffer before parsing:
        result = fgets(buffer, MAX_INPUT_SIZE, stdin);
        if(result == NULL && ferror(stdin)){
            errExit("Error reading from stdin using fgets!");
        }

        if(!feof(stdin)){
            // Check for content to parse:
            if(buffer[0] != 0){
                int cmdResult = processCmd(buffer, envp);
                if(cmdResult == STATUS_EXIT){
                    result = 0;
                }
            }
        }

        ++cmdCounter;
    }
    free(user);
    user = NULL;
    printf("Good-bye!\n");
    return 0;
}
