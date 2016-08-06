#include "mini-shell.h"

// Main function sets up permanent variables and runs a main loop
// prompting for and processing input from stdin
int main(int argc, char** argv){
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
            for(int i = 0; words[i] != 0; ++i){
                printf("'%s'\n", words[i]);
            }
            free((void**)(words));
            words = NULL;
        }

        ++cmdCounter;
    }
    free(user);
    user = NULL;
    printf("Exiting!");
    return 0;
}
