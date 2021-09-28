#include <stdbool.h> // oh god why
#include <stdio.h>

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

        printf("What was inputted: ");
        printf(input);
        printf("\n");
        break;
    }

    return 0;
}
