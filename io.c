#include <stdlib.h>
#include "io.h"

void requestName(char* buffer) {
    printf("Provide user name: ");
    if (fgets(buffer, MAX_STRING_LEN, stdin) == NULL)
        die_with_error("Fail to get name");
    buffer[strlen(buffer)-1] = 0;
}

void printPrompt() {
    printf("<%s> : ", name);
    fflush(stdout);
}

int readMessage(char* buffer, int maxSize) {
    printPrompt();
    if (fgets(buffer, maxSize, stdin) == NULL)
        die_with_error("Fail to get message or command");
    int size = strlen(buffer)-1;
    buffer[size] = 0;
    return size;
}

void printRecievedMessage(char* message) {
    printf("\r                                           \r");
    printf("%s\n", message);
    printPrompt();
}

int sendMessage(int socket, char* message, int size) {
    return send(socket , message , size , 0);
}

int recieveMessage(int sock, char* buffer) {    
    int numByte = recv(sock , buffer , MAX_STRING_LEN , 0);
    if (numByte >= 0)
        buffer[numByte] = 0;
    return numByte;
}

void die_with_error(char* error_message) {
    perror(error_message);
    exit(1);
}


