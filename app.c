#include <stdlib.h>
#include "app.h"

#define BUFFER_SIZE 1024
#define FILENAME_BUFFER_SIZE 128


#define EXIT 0
#define MESSAGE 1
#define NAME 10
#define FILENAME 2
#define FILEDATA 3
#define FILEEND 4

static char name[BUFFER_SIZE];

static void printPrompt() {
    printf("<%s> : ", name);
    fflush(stdout);
}

static int readMessage(char* buffer, int maxSize) {
    printPrompt();
    if (fgets(buffer, maxSize, stdin) == NULL)
        perror("Fail to get message or command");
    int size = strlen(buffer);
    buffer[size-1] = 0;
    return size;
}

static int processAndSend(char* buffer, int size, int (*sendData)(char*, int)) {
    // printf("%s\n%d\n", buffer, size);
    if (strcmp(buffer+1, "exit") == 0) {
        buffer[0] = EXIT;
        strcpy(buffer+1, name);
        sendData(buffer, strlen(name) + 2);
        exit(0);
    } else if (strncmp(buffer+1, "load ", 5) == 0) {
        int fileNameLen = strlen(buffer+6);
        char fileName[fileNameLen + 1];
        strcpy(fileName, buffer+6);
        FILE* file = fopen(fileName, "r");

        // send file name
        buffer[0] = FILENAME;
        strcpy(buffer+1, fileName);
        if (sendData(buffer, fileNameLen + 2))
            return 1;
        // send file data
        buffer[0] = FILEDATA;
        while (1) {
            int size = fread(buffer+1, 1, BUFFER_SIZE-1, file);
            if (size <= 0)
                return 1;
            sendData(buffer, size+1);
        }
        // signal the end of a file
        buffer[0] = FILEEND;
        sendData(buffer, 1);
    } else {
        buffer[0] = MESSAGE;
        if (sendData(buffer, size) < 0)
            return 1;
    }
    return 0;
}


void *send_func(void *data_struct) {
    SendDataFun* sendData = (SendDataFun*) data_struct;
    
    char buffer[BUFFER_SIZE];
    buffer[0] = NAME;
    printf("Provide user name: ");
    if (fgets(buffer+1, BUFFER_SIZE-1, stdin) == NULL) {
        perror("Fail to get name");
        exit(3);
    }
    if (sendData(buffer, strlen(buffer+1) + 2) < 0) {
        perror("error sending name");
        exit(2);
    }

    while (1) { /* run until user enters "." to quit. */
        int size = readMessage(buffer+1, BUFFER_SIZE-1);
        if (size < 0)
            break;
        if (processAndSend(buffer, size, sendData))
            break;
    }
    return NULL;
}

static void onRecieveData(char* data, int size) {
    if (data[0] == MESSAGE) {
        data[size] = 0;
        printf("\r                                           \r");
        printf("%s\n", data+1);
        printPrompt();
    } else if (data[0] == NAME) {
        printf("%s is entering the chat", data+1);
    } else if (data[0] == FILENAME) {
        char fileName[FILENAME_BUFFER_SIZE];
        strcpy(fileName, data+1);
        FILE* file = fopen(fileName, "w");
        while(1){
            getNextPacket(&data, &size);
            if (size < 0) {
                perror("size negative");
                exit(2);
            }
            if (data[0] == FILEEND)
                break;
            if (data[0] != FILEDATA) {
                fprintf(stderr, "Wrong file passing prototype: %d\n", data[0]);
                exit(2);
            }
            fwrite(data+1, 1, size-1, file);
            free(data);
        }
    } else {
        fprintf(stderr, "Unrecognized message type %d\n", data[0]);
        exit(2);
    }
}

void *recv_func(void *data_struct) {
    char* data;
    int size;
    while(1){
        getNextPacket(&data, &size);
        if (size < 0) {
            perror("size negative");
            break;
        }
        onRecieveData(data, size);
        free(data);
    }
    exit(0);
}