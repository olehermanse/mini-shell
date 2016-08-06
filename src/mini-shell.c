#include "mini-shell.h"

void ** freeArray(void ** pointerArray){
    assert(pointerArray);
    for( int i = 0; pointerArray[i] != 0; ++i){
        free(pointerArray[i]);
        pointerArray[i] = 0;
    }
    free(pointerArray);
    pointerArray = 0;
    return pointerArray;
}

char** strcpyArray(char** strings){
    int count = 0;
    while(strings[count++]);
    char** newArray = malloc(sizeof(char*)*count);
    count = 0;
    while(strings[count]){
        newArray[count] = malloc(sizeof(char)*(1+strlen(strings[count])));
        strcpy(newArray[count], strings[count]);
        ++count;
    }
    newArray[count] = 0;
    return newArray;
}

char** parseString(char* buffer){
    char* tokens[32];
    int index = 0;
    const char sep[] = " \t\n\v\f\r";
    char* token = strtok(buffer, sep);
    while(token){
        tokens[index++] = token;
        token = strtok(NULL, sep);
    }
    tokens[index++] = NULL;
    char** returnTokens = malloc(sizeof(char*)*index);
    index = 0;
    while(tokens[index]){
        returnTokens[index] = tokens[index];
        ++index;
    }
    returnTokens[index] = NULL;
    return returnTokens;
}

int main(int argc, char** argv){
    int bufferSize = 255;
    char* buffer = (char*)(malloc(sizeof(char)*bufferSize));
    char* result = buffer;
    char* user = NULL;
    int cmdCounter = 0;

    strcpy(buffer,getenv("USER"));
    user = (char*)malloc(strlen(buffer)+1);
    assert(user);
    strcpy(user, buffer);
    while(result){
        printf("%s@mini-shell %i> ", user, cmdCounter);
        result = fgets(buffer, bufferSize, stdin);
        if(buffer[0] != 0){
            char** words = parseString(buffer);
            for(int i = 0; words[i] != 0; ++i){
                printf("'%s'\n", words[i]);
            }
            free((void**)(words));
            words = NULL;
        }
        ++cmdCounter;
    }
    free(buffer);
    buffer = NULL;
    result = NULL;
    free(user);
    user = NULL;
    printf("Exiting!");
    return 0;
}
