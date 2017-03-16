#include <stdlib.h>
#include "app.h"

typedef struct 
{
    char name[100];
    FILE* file;
} FileInfo;


static char name[MAX_STRING_LEN];

static void requestName(char* buffer) {
    printf("Provide user name: ");
    if (fgets(buffer, MAX_STRING_LEN, stdin) == NULL)
        perror("Fail to get name");
    buffer[strlen(buffer)-1] = 0;
}

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

static void printRecievedMessage(char* message) {
    printf("\r                                           \r");
    printf("%s\n", message);
    printPrompt();
}

static int parseLoadFileName(FileInfo* info, char* buffer) {
    if (strncmp(buffer, "load ", 5) == 0) {
        strcpy(info->name, buffer+5);
        info->file = fopen(info->name, "r");
        return 1;
    }
    return 0;
}


static int processAndSend(char* buffer, int size, int (*sendData)(char*, int)) {
    FileInfo info;
    // printf("%s\n%d\n", buffer, size);
    if (parseLoadFileName(&info, buffer)) {
        // send file name
        buffer[0] = 0;
        buffer[1] = 1;
        strcpy(buffer+2, info.name);
        info.file = fopen(info.name, "r");
        if (sendData(buffer, strlen(info.name) + 2))
            return 1;
        // send file data
        while (1) {
            int size = fread(buffer, 1, MAX_STRING_LEN-2, info.file);
            if (size <= 0)
                return 1;
            sendData(buffer, size+2);
        }
        // signal the end of file transfer
        buffer[0] = 0;
        buffer[1] = 0;
        sendData(buffer,2);
    } else {
        if (sendData(buffer, size) < 0)
            return 1;
    }
    return 0;
}


void *send_func(void *data_struct) {
    SendDataFun* sendData = (SendDataFun*) data_struct;
    
    requestName(name);

    if (sendData(name, strlen(name)) < 0)
        perror("error sending name");

    char buffer[MAX_STRING_LEN];
    while (1) { /* run until user enters "." to quit. */
        int size = readMessage(buffer, MAX_STRING_LEN);
        if (size < 0)
            break;
        if (processAndSend(buffer, size, sendData))
            break;
    }
    return NULL;
}



static void onRecieveData(FileInfo* info, char* data, int size) {
    if (data[0] == 0) {
        if (data[1] == 1) {
            if (info->name[1] == 0) {
                strcpy(info->name, data+2);
            } else {
                fwrite(data, 1, size, info->file);
            }
        } else {
            // end recieving
            info->name[0] = 0;
            fclose(info->file);
        }
    } else {
        data[size] = 0;
        printRecievedMessage(data);
    }
}

void *recv_func(void *data_struct) {
    // SendDataFun* sendData = (SendDataFun*) data_struct;
    FileInfo info;
    while(1){
        char* data;
        int size;
        getNextPacket(&data, &size);
        if (size < 0) {
            perror("size negative");
            break;
        }
        onRecieveData(&info, data, size);
        free(data);
    }
    exit(0);
}