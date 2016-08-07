#include "string-funcs.h"

// Loops through null terminated string and replaces all occurences
// of "match" with "replace"
// Return: no. of characters replaced
int strReplace(char* str, char match, char replace){
    assert(str);
    int count = 0;
    for(int i = 0; str[i] != 0; ++i){
        if (str[i] == match) {
            str[i] = replace;
            ++count;
        }
    }
    return count;
}

// Loop to free all cstrings in array of cstrings as well as array itself.
// Return: NULL pointer for assignment.
void ** freeArray(void ** pointerArray){
    if(!pointerArray){
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
    assert(words);
    int count = 0;
    while(words[count]){
        ++count;
    }
    return count;
}

// Return: int index of first of any character from chars in str
int indexOf(char* str, const char* chars){
    for(int i = 0; str[i] != 0; ++i){
        for(int j = 0; chars[j] != 0; ++j){
            if(str[i] == chars[j])
                return i;
        }
    }
    return -1;
}

// Prints an  array of strings
void printStrings(char** strs){
    int index = 0;
    if(strs != NULL){
        for(index = 0; strs[index] != NULL; ++index){
            if(index == 0){
                printf("Strings:");
            }else{
                printf(",");
            }
            printf("'%s'", strs[index]);
        }
    }
    if(index == 0){
        printf("No strings");
    }
    printf("\n");
}

// Parses a string using strtok. String is split into tokens("words")
// Return: null terminated array(pointer) of cstrings.
// mallocs: array pointer only (strings point directly to buffer)
char** splitString(char* buffer, const char* sep, const char* substrsyms){
    assert(buffer);
    assert(sep);

    const int MAX_TOKENS = 25;
    char* tokens[MAX_TOKENS+1];
    int index = 0;

    // Parse string, saved tokens point directly to buffer:
    char* token = strtok(buffer, sep);
    const char SINGLE_QUOTE[] = "\'";
    const char DOUBLE_QUOTE[] = "\"";

    while(token && index < MAX_TOKENS){
        if(token[0] == SINGLE_QUOTE[0]){
            token[strlen(token)] = ' ';
            token = strtok(token, SINGLE_QUOTE);
        }else if(token[0] == DOUBLE_QUOTE[0]){
            token[strlen(token)] = ' ';
            token = strtok(token, DOUBLE_QUOTE);
        }
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
