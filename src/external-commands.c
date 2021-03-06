#include "external-commands.h"

// Checks results from fork() and waitpid() system calls
// Prints error message if needed and sets appropriate status
// Return: int returnStatus (See enum in definitions.h)
en_status parentResult(pid_t pid1, pid_t pid2, int status){
    if(pid1 != pid2){
        errPrint("Problem with fork: pids %i != %i.\n", pid1, pid2);
        return STATUS_SOFT_FAILURE;
    }
    if (WIFEXITED(status)){
        return STATUS_SUCCESS;
    }else if (WIFSIGNALED(status)){
        errPrint("fork: Child ended because of an uncaught signal.\n");
        return STATUS_SOFT_FAILURE;
    }else if (WIFSTOPPED(status)){
        errPrint("fork: Child process has stopped.\n");
        return STATUS_SOFT_FAILURE;
    }
    errPrint("fork: Unknown error!");
    return STATUS_SOFT_FAILURE;
}

// Fork and execve, don't wait, exit. Used to get rid of zombie processes.
// Parent waits for this process. Resulting in orphan child process.
en_status secondFork(char* path, char** argv, int argc, char* envp[], int* fds){
    pid_t pid = fork();
    if(pid == 0){
        // Child:
        if(fds){
            // Redirect input:
            if(fds[0] != 0)
                dup2(fds[0], 0);
            // Redirect output:
            if(fds[1] != 1)
                dup2(fds[1], 1);
        }
        return execve(path, argv, envp);
    }else if(pid > 0){
        // Middle Parent (Exits immediately after fork()):
        printf("Child process started: [%i].\n", pid);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}

// Ampersand & - Fork and execve without waiting for child process to finish
// Return: int returnStatus (See enum in definitions.h)
en_status forkExecNoWait(char* path, char** argv, int argc, char* envp[], int* fds){
    int status;
    pid_t pid = fork();
    if(pid == 0){
        // Child:
        secondFork(path, argv, argc, envp, fds);
    }else if(pid > 0){
        // Parent:
        pid_t pid2 = pid;
        pid2 = waitpid(pid, &status, 0);
        return parentResult(pid, pid2, status);
    }
    errPrint("Problem with fork: Returned %i.\n", pid);
    return STATUS_SOFT_FAILURE;
}

// Regular fork execve (no &)
// Return: int returnStatus (See enum in definitions.h)
en_status forkExecWait(char* path, char** argv, int argc, char *envp[], int* fds){
    int status = 0;

    pid_t pid = fork();
    if(pid == 0){
        // Child:
        if(fds){
            // Redirect input:
            if(fds[0] != 0)
                dup2(fds[0], 0);
            // Redirect output:
            if(fds[1] != 1)
                dup2(fds[1], 1);
        }
        return execve(path, argv, envp);
    }else if(pid > 0){
        // Parent:
        pid_t pid2 = pid;
        pid2 = waitpid(pid, &status, 0);
        return parentResult(pid, pid2, status);
    }
    errPrint("Problem with fork: Returned %i.\n", pid);
    return STATUS_SOFT_FAILURE;
}

// Check if there is an executable at path
// Return: bool true if executable
bool checkExecutable(char* path){
    assert(path);
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
en_status findExecutable(char* name, char* pathOutput){
    // Check for executable directly
    int res = 0;
    res = sprintf(pathOutput, "%s", name);
    assert(res >= 0);
    if(checkExecutable(pathOutput)){
        return STATUS_SUCCESS;
    }

    // Get copy of PATH environment variable:
    const char* PATH = getenv("PATH");
    char pathVar[MAX_PATH_SIZE];
    strcpy(pathVar, PATH);

    // Parse PATH using strtok
    char* token = strtok(pathVar, ":");
    res = sprintf(pathOutput, "%s/%s", token, name);
    assert(res >= 0);
    if(checkExecutable(pathOutput)){
        return STATUS_SUCCESS;
    }
    while(token){
        token = strtok(NULL, ":");
        res = sprintf(pathOutput, "%s/%s", token, name);
        assert(res >= 0);
        if(checkExecutable(pathOutput)){
            return STATUS_SUCCESS;
        }
    }
    return STATUS_FAILURE;
}

// Searches for an executable, forks and executes
// Doesn't wait for child if & is found
// Return: returnStatus from called function(s)
en_status processExternalCmd(char** argv, int num, char* envp[], int* fds){
    bool willWait = true;

    char* lastArgument = argv[num-1];
    assert(lastArgument);
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
    en_status cmdResult = findExecutable(argv[0], fullPath);
    if(cmdResult == STATUS_SUCCESS){
        if(willWait){
            cmdResult = forkExecWait(fullPath, argv, num, envp, fds);
        }else{
            cmdResult = forkExecNoWait(fullPath, argv, num, envp, fds);
        }
    }else{
        printf("Unrecognized command: '%s'\n", argv[0]);
    }
    return cmdResult;
}
