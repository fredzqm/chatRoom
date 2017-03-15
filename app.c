#include <stdlib.h>
#include "app.h"

void requestName(char* buffer) {
    printf("Provide user name: ");
    if (fgets(buffer, MAX_STRING_LEN, stdin) == NULL)
        perror("Fail to get name");
    buffer[strlen(buffer)-1] = 0;
}

void printPrompt() {
    printf("<%s> : ", name);
    fflush(stdout);
}

int readMessage(char* buffer, int maxSize) {
    printPrompt();
    if (fgets(buffer, maxSize, stdin) == NULL)
        perror("Fail to get message or command");
    int size = strlen(buffer)-1;
    buffer[size] = 0;
    return size;
}

void printRecievedMessage(char* message) {
    printf("\r                                           \r");
    printf("%s\n", message);
    printPrompt();
}




