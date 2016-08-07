#include "externalcommands.h"

// Checks results from fork() and waitpid() system calls
// Prints error message if needed and sets appropriate status
// Return: int returnStatus (See enum in definitions.h)
int parentResult(pid_t pid1, pid_t pid2, int status){
    if(pid1 != pid2){
        errPrint("Problem with fork: pids %i != %i.\n", pid1, pid2);
        return STATUS_OK;
    }
    if (WIFEXITED(status)){
        return STATUS_SUCCESS;
    }else if (WIFSIGNALED(status)){
        errPrint("fork: Child ended because of an uncaught signal.\n");
        return STATUS_OK;
    }else if (WIFSTOPPED(status)){
        errPrint("fork: Child process has stopped.\n");
        return STATUS_OK;
    }
    errPrint("fork: Unknown error!");
    return STATUS_OK;
}

// Fork and execve, don't wait, exit. Used to get rid of zombie processes.
// Parent waits for this process. Resulting in orphan child process.
void secondFork(char* path, char** argv, int argc, char* envp[]){
    pid_t pid = fork();
    if(pid == 0){
        // Child:
        execve(path, argv, envp);
    }else if(pid > 0){
        // Middle Parent (Exits immediately after fork()):
        printf("Child process started: [%i].\n", pid);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

// Ampersand & - Fork and execve without waiting for child process to finish
// Return: int returnStatus (See enum in definitions.h)
int forkExecNoWait(char* path, char** argv, int argc, char* envp[]){
    int status;
    pid_t pid = fork();
    if(pid == 0){
        // Child:
        secondFork(path, argv, argc, envp);
    }else if(pid > 0){
        // Parent:
        pid_t pid2 = pid;
        pid2 = waitpid(pid, &status, 0);
        return parentResult(pid, pid2, status);
    }
    errPrint("Problem with fork: Returned %i.\n", pid);
    return STATUS_OK;
}

// Regular fork execve (no &)
// Return: int returnStatus (See enum in definitions.h)
int forkExecWait(char* path, char** argv, int argc, char *envp[]){
    int status = 0;

    pid_t pid = fork();
    if(pid == 0){
        // Child:
        execve(path, argv, envp);
    }else if(pid > 0){
        // Parent:
        pid_t pid2 = pid;
        pid2 = waitpid(pid, &status, 0);
        return parentResult(pid, pid2, status);
    }
    errPrint("Problem with fork: Returned %i.\n", pid);
    return STATUS_OK;
}

// Check if there is an executable at path
// Return: bool true if executable
bool checkExecutable(char* path){
    struct stat sb;
    int result = stat(path, &sb);
    if(result == 0){
        if(sb.st_mode & S_IXUSR){
            return true;
        }
    }
    return false;
}

// Searches working working directory and directories in PATH for executable
// Return: STATUS_SUCCESS if executable found, STATUS_FAILURE otherwise
int findExecutable(char* name, char* pathOutput){
    // Check for executable directly
    sprintf(pathOutput, "%s", name);
    if(checkExecutable(pathOutput)){
        return STATUS_SUCCESS;
    }

    // Check for executable in this folder:
    sprintf(pathOutput, "./%s", name);
    if(checkExecutable(pathOutput)){
        return STATUS_SUCCESS;
    }

    // Get copy of PATH environment variable:
    const char* PATH = getenv("PATH");
    char pathVar[512];
    strcpy(pathVar, PATH);

    // Parse PATH using strtok
    char* token = strtok(pathVar, ":");
    sprintf(pathOutput, "%s/%s", token, name);
    if(checkExecutable(pathOutput)){
        return STATUS_SUCCESS;
    }
    while(token){
        token = strtok(NULL, ":");
        sprintf(pathOutput, "%s/%s", token, name);
        if(checkExecutable(pathOutput)){
            return STATUS_SUCCESS;
        }
    }
    return STATUS_FAILURE;
}

// Searches for an executable, forks and executes
// Doesn't wait for child if & is found
// Return: returnStatus from called function(s)
int processExternalCmd(char** argv, int num, char* envp[]){
    bool willWait = true;

    char* lastArgument = argv[num-1];
    int length = strlen(lastArgument);
    if(lastArgument[length-1] == '&'){
        lastArgument[length-1] = 0;
        if(length == 1){
            argv[num-1] = NULL;
            --num;
        }
        willWait = false;
    }
    char fullPath[256];
    int cmdResult = findExecutable(argv[0], fullPath);
    if(cmdResult == STATUS_SUCCESS){
        if(willWait){
            cmdResult = forkExecWait(fullPath, argv, num, envp);
        }else{
            cmdResult = forkExecNoWait(fullPath, argv, num, envp);
        }
    }else{
        printf("Unrecognized command: '%s'\n", argv[0]);
    }
    return cmdResult;
}
