#include "mini-shell.h"

// Processes a query by splitting and attempting to run built in or
// external command.
// Return: int returnStatus from called function(s) (See enum in definitions.h)
int processCmd(char* buffer, char* envp[], int* fds){
    assert(buffer);

    char** argv = splitString(buffer, " \t\n\v\f\r");
    int num = wordCount(argv);
    if(num == 0){
        free(argv);
        return STATUS_SUCCESS;
    }

    int cmdResult = processBuiltinCmd(argv, fds);
    if(cmdResult == STATUS_UNRECOGNIZED){
        cmdResult = processExternalCmd(argv, num, envp, fds);
    }
    free((void**)(argv));
    argv = NULL;
    return cmdResult;
}

int pipeCmds(char* buffer, char* envp[]){
    /*int newlines = */strReplace(buffer, '\n', 0);
    char** commands = splitString(buffer, "|");
    int num = wordCount(commands);

    if(num == 0){
        return STATUS_SUCCESS;
    }
    if(num == 1){
        int cmdResult = processCmd(commands[0], envp, NULL);
        free(commands);
        commands = 0;
        return cmdResult;
    }

    int newPipe[2] = {0,0};
    int fd[2];
    int index;
    for(index = 0; index < num - 1; ++index){
        fd[0] = newPipe[0];
        pipe(newPipe);
        fd[1] = newPipe[1];
        processCmd(commands[index], envp, fd);
        if(index > 0)
            close(fd[0]);
        close(fd[1]);
    }
    fd[0] = newPipe[0];
    fd[1] = 1;
    processCmd(commands[index], envp, fd);
    close(fd[0]);
    free(commands);
    commands = 0;
    return STATUS_SUCCESS;
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
        int c = fgetc(stdin);
        int i = 0;
        buffer[i++] = c;
        while( c != '\n' && c != EOF && c != 0){
            buffer[i++] = c = fgetc(stdin);
        }
        if( c == EOF ){
            result = 0;
        }
        if( c == 0 ){
            buffer[i] = 0;
        }

        if(result == NULL && ferror(stdin)){
            errExit("Error reading from stdin using fgets!");
        }


        if(!feof(stdin)){
            // Check for content to parse:
            if(buffer[0] != 0){
                int cmdResult = pipeCmds(buffer, envp);
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
