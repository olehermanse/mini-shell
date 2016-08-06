#include "mini-shell.h"

char** parseLine(char* buffer){
    int index = 0;
    char c = buffer[index];

    while(isspace(c) && c != 0){
        c = buffer[++index];
    }
    if(c == 0){
        return 0;
    }

    typedef enum {CHARACTER, WHITESPACE} parseState;
    parseState state = CHARACTER;

    const int MAX_WORDS = 32;
    char* words[MAX_WORDS+1];
    int wordsCounter = 0;

    char word[128];
    char wordIndex = 0;
    while(c != 0 && wordsCounter <= MAX_WORDS){
        if(state == CHARACTER){
            if(isspace(c)){
                state = WHITESPACE;
                word[wordIndex++] = 0;
                words[wordsCounter] = (char*)(malloc(wordIndex));
                strcpy(words[wordsCounter], word);
                ++wordsCounter;
                wordIndex = 0;
            }else{
                word[wordIndex++] = c;
            }
        }else if(state == WHITESPACE){
            if(!isspace(c)){
                state = CHARACTER;
                word[wordIndex++] = c;
            }
        }
        c = buffer[++index];
    }
    char** returnWords = (char**)(malloc( (wordsCounter+1) * sizeof(char*) ));
    returnWords[wordsCounter] = NULL;
    for(int i = 0; i<wordsCounter; ++i){
        returnWords[i] = words[i];
    }
    return returnWords;
}

void ** nestedFree(void ** pointerArray){
    assert(pointerArray);
    for( int i = 0; pointerArray[i] != 0; ++i){
        free(pointerArray[i]);
        pointerArray[i] = 0;
    }
    free(pointerArray);
    pointerArray = 0;
    return pointerArray;
}

int main(int argc, char** argv){
    int bufferSize = 255;
    char buffer[bufferSize];
    long result = 1;
    char* user = NULL;
    int cmdCounter = 0;

    strcpy(buffer,getenv("USER"));
    user = (char*)malloc(strlen(buffer)+1);
    assert(user);
    strcpy(user, buffer);

    while(result){
        printf("%s@mini-shell %i> ", user, cmdCounter);   // Print standard line beginning
        result = (long)fgets(buffer, bufferSize, stdin);
        //Only execute and save lines that are not empty
        if(buffer[0] != 0){
            char** words = parseLine(buffer);
            for(int i = 0; words[i] != 0; ++i){
                printf("'%s'\n", words[i]);
            }
            nestedFree((void**)(words));
            result = 1;
        }
        ++cmdCounter;
    }
    free(user);
    return 0;
}
