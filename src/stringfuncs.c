#include "stringfuncs.h"

// Loop to free all cstrings in array of cstrings as well as array itself.
// Return: NULL pointer for assignment.
void ** freeArray(void ** pointerArray){
    assert(pointerArray);
    if(pointerArray == NULL){
        return NULL;
    }
    for( int i = 0; pointerArray[i] != 0; ++i){
        free(pointerArray[i]);
        pointerArray[i] = NULL;
    }
    free(pointerArray);
    pointerArray = NULL;
    return pointerArray;
}

// Copies a null terminated array of null terminated.
// strings(cstrings) to a newly mallocd similar structure.
// Return: new copied null terminated array of cstrings.
// mallocs: all strings as well as array.
char** strcpyArray(char** strings){
    assert(strings);
    if(strings == NULL){
        return NULL;
    }
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

// Counts words in null terminated array(/pointer)
// Return: count of words(excluding null terminator)
int wordCount(char** words){
    int count = 0;
    while(words[count]){
        ++count;
    }
    return count;
}

// Parses a string using strtok. String is split into tokens("words")
// Return: null terminated array(pointer) of cstrings.
// mallocs: array pointer only (strings point directly to buffer)
char** splitString(char* buffer, const char* sep){
    assert(buffer);
    assert(sep);
    if(buffer == NULL || sep == NULL){
        return NULL;
    }

    const int MAX_TOKENS = 31;
    char* tokens[MAX_TOKENS+1];
    int index = 0;

    // Parse string, saved tokens point directly to buffer:
    char* token = strtok(buffer, sep);
    while(token && index < MAX_TOKENS){
        tokens[index++] = token;
        token = strtok(NULL, sep);
    }
    tokens[index++] = NULL; // NULL terminated array

    // Allocate array of cstrings:
    char** returnTokens = malloc(sizeof(char*)*index);

    // Copy pointers, not content:
    index = 0;
    while(tokens[index]){
        returnTokens[index] = tokens[index];
        ++index;
    }
    returnTokens[index] = NULL; // NULL terminated array

    return returnTokens;
}
