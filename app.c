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
    int size = strlen(buffer);
    buffer[size-1] = 0;
    return size;
}

void printRecievedMessage(char* message) {
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


int processAndSend(char* buffer, int size, int (*sendData)(char*, int)) {
    FileInfo info;
    // printf("%s\n%d\n", buffer, size);
    if (parseLoadFileName(&info, buffer)) {
        // send file name
        buffer[0] = 0;
        buffer[1] = 1;
        strcpy(buffer+2, info.name);
        info.file = fopen(info.name, "r");
        if (sendData(buffer, strlen(info.name) + 2))
            return -1;
        // send file data
        while (1) {
            int numbytes = fread(buffer, 1, MAX_STRING_LEN-2, info.file);
            if (numbytes <= 0)
                return -1;
            sendData(buffer, numbytes+2);
        }
        // signal the end of file transfer
        buffer[0] = 0;
        buffer[1] = 0;
        sendData(buffer,2);
    } else {
        if (sendData(buffer, size) < 0)
            return -1;
    }
    return 0;
}


FileInfo info;
void _onRecieveBroadcast(char* data, int size) {
    if (data[0] == 0) {
        if (data[1] == 1) {
            if (info.name[1] == 0) {
                strcpy(info.name, data+2);
            } else {
                fwrite(data, 1, size, info.file);
            }
        } else {
            // end recieving
            info.name[0] = 0;
            fclose(info.file);
        }
    } else {
        data[size] = 0;
        printRecievedMessage(data);
    }
    // printf("%s\n%d\n", data, size);
}
