#include <stdbool.h> // oh god why
#include <stdio.h>
#include <string.h>

int LINE_LENGTH = 100; // Max # of characters in an input line
int MAX_ARGS = 7; // Max number of arguments to a command
int MAX_LENGTH = 20; // Max # of characters in an argument
int MAX_PT_ENTRIES = 32 ;// Max entries in the Process Table

int main(int argc, char const *argv[]) {
    bool continueRunning = true;
    printf("Welcome to Shell379 - Syjiao\n");

    while (continueRunning) {
        printf("Please enter a command\n");
        char input[LINE_LENGTH];
        scanf("%[^\n]%*c", input);
        char delim[] = " ";
        char *ptr = strtok(input, delim);
        
        char* args[MAX_ARGS][1];
        int j = 0;
        while (ptr != NULL && j < 7) {
            args[j++][0] = ptr;
            ptr = strtok(NULL, delim);
        }

        if (strcmp(args[0], "exit")) {
            // TODO: implement exit and cleanup
            printf("We hit exit.\n");
        }
        else if (strcmp(args[0], "jobs")) {
            
        }

        break;
    }

    return 0;
}
