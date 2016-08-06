#include "mini-shell.h"

// Attempt to execute program at path:
// Returns on failure, does not return on success
int tryExecve(char* path, char** argv, char** envp){
    execve(path, argv, envp);
    return 0;
}

// This function is called by a safeforked child process:
int childProcess(char** param, int words, char *envp[], pid_t pid){
    assert(param);
    assert(param[0]);

    // Get path variable, will look through all folders specified in path:
    char* path = getenv("PATH");
    int pathIndex = 0;

    // The full program path(folder + name) will be stored in pathBuffer:
    char pathBuffer[256];
    int pbIndex = 0;
    bzero(pathBuffer, 256);

    // "Boolean" variables to keep track of when done
    char done = 0;
    char lastOne = 0;

    // dprint("Path variable : %s\n", path);   //Print path variable

    // Loop through all paths in path variable:
    while(!done){
        if(path[pathIndex] == ':' || path[pathIndex] == 0){   // Separated by null or :
            sprintf(&(pathBuffer[pbIndex]), "/%s", param[0]); // Add param(program name)
            tryExecve(pathBuffer, param, envp);               // Attempt to execute path
            pbIndex = 0;                                      // Reset pathbuffer
        }else{
            pathBuffer[pbIndex++] = path[pathIndex];          // Copy from path variable
        }

        // Loop one more when null byte is encountered, for last path
        if(lastOne == 1)
            done = 1;
        if(path[pathIndex] == 0)
            lastOne = 1;
        ++pathIndex;
    }

    // Try bin folder in ifish directory:
    sprintf(pathBuffer, "./bin/%s", param[0]);
    tryExecve(pathBuffer, param, envp);
    bzero(pathBuffer, 256);

    // Try in same directory as ifish:
    sprintf(pathBuffer, "./%s", param[0]);
    tryExecve(pathBuffer, param, envp);
    bzero(pathBuffer, 256);

    // Try just the string provided:
    sprintf(pathBuffer, "%s", param[0]);
    tryExecve(pathBuffer, param, envp);
    bzero(pathBuffer, 256);

    // Error message:
    printf("ifish: %s: command not found\n", param[0]);
    _exit(0); // Exit current process only.
}

int parentProcess(char** words, int num, char* envp[], pid_t pid, BOOL willWait){
    int status = 0;
    if(willWait == TRUE)
        waitpid(pid, &status, 0);
    else
        printf("Child process ID: %i\n", pid);
    return STATUS_SUCCESS;
}

// Executes a command that's already been split into words by processLine():
int execute(char** argv, int argc, char *envp[], BOOL willWait){

    pid_t pid = fork();

    if(pid == -1){
        printf("Problem with fork: Could not fork\n");
        return STATUS_FAILURE;
    }else if(pid == 0){
        return childProcess(argv, argc, envp, pid);
    }else if(pid > 0){
        return parentProcess(argv, argc, envp, pid, willWait);
    }else{
        printf("Problem with fork: Returned %i.\n", pid);
        return 0;
    }
    return STATUS_FAILURE;
}

int processLine(char* buffer, char** words, int num, char* envp[]){
    if(num == 0){
        return STATUS_UNRECOGNIZED;
    }
    assert(buffer);
    assert(words);
    assert(words[0]);

    long result = 0;
    BOOL willWait = TRUE;

    char* lastWord = words[num-1];
    int lastWordLength = strlen(lastWord);
    if(lastWord[lastWordLength-1] == '&'){
        lastWord[lastWordLength-1] = 0;
        if(lastWordLength == 1){
            words[num-1] = NULL;
            --num;
        }
        willWait = FALSE;
    }

    result = (long)execute(words, num, envp, willWait);

    return result;
}

// Main function sets up permanent variables and runs a main loop
// prompting for and processing input from stdin
int main(int argc, char** argv, char *envp[]){
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

        // Check for content to parse:
        if(buffer[0] != 0){
            // Parse and process string input
            char** words = splitString(buffer, " \t\n\v\f\r");
            int num = wordCount(words);
            if(num){
                int cmdResult = processBuiltinCmd(words);
                if(cmdResult == STATUS_UNRECOGNIZED){
                    cmdResult = processLine(buffer, words, num, envp);
                }
                if(cmdResult == STATUS_EXIT){
                    result = 0;
                }
            }
            free((void**)(words));
            words = NULL;
        }

        ++cmdCounter;
    }
    free(user);
    user = NULL;
    printf("Goodbye!\n");
    return 0;
}
