#include "mini-shell.h"

void cleanBuffer(char* buf){
    assert(buf);
    int index = strlen(buf)-1;
    if(buf[index] == '\n'){
        buf[index] = 0;
    }
}

int main(int argc, char** argv){
    int bufferSize = 255;
    char buffer[bufferSize];
    long result = 1;
    char* user = 0;
    int cmdCounter = 0;

    strcpy(buffer,getenv("USER"));
    user = (char*)malloc(strlen(buffer)+1);
    assert(user);
    strcpy(user, buffer);


    while(result){
        printf("%s@mini-shell %i> ", user, cmdCounter);   // Print standard line beginning
        result = (long)fgets(buffer, bufferSize, stdin);
        cleanBuffer(buffer);
        //Only execute and save lines that are not empty
        if(buffer[0] != 0){
            result = 1;//processLine(buffer, mp, envp);  // Process line and execute
        }
        ++cmdCounter;
    }
    printf("Hello Shell!\n");
    return 0;
}
