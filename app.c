#include <stdlib.h>
#include "app.h"

#define BUFFER_SIZE 1024
#define FILENAME_BUFFER_SIZE 128


#define MESSAGE 1
#define FILENAME 2
#define FILEDATA 3
#define FILEEND 4

static char name[BUFFER_SIZE];

static void clearLine() {
    printf("\r                                           \r");
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

static int processAndSend(char* buffer, int size, int (*sendData)(char*, int)) {
    char sent[BUFFER_SIZE];
    if (strcmp(buffer, "exit") == 0) {
        sent[0] = MESSAGE;
        sprintf(sent+1, "%s exited the chat", name);
        sendData(sent, strlen(sent+1) + 2);
        exit(0);
    } else if (strncmp(buffer, "load ", 5) == 0) {
        int fileNameLen = strlen(buffer+5);
        char fileName[fileNameLen + 1];
        strcpy(fileName, buffer+5);
        FILE* file = fopen(fileName, "r");
        if (file == NULL) {
            perror("Fail to open file");
            exit(3);
        }
        // send file name
        sent[0] = FILENAME;
        strcpy(sent+1, fileName);
        if (sendData(sent, fileNameLen + 2) < 0)
            return 1;

        printf("Sending file \"%s\"", fileName);
        // send file data
        sent[0] = FILEDATA;
        while (1) {
            printf("."); fflush(stdout);
            int size = fread(sent+1, 1, BUFFER_SIZE-1, file);
            if (size <= 0) {
                break; // end of file
            }
            if (sendData(sent, size+1) < 0)
                return 1;
        }
        printf("\n");
        fclose(file);
        // confirm file sent
        sent[0] = FILEEND;
        sendData(sent, 1);
    } else {
        sent[0] = MESSAGE;
        sprintf(sent+1, "<%s> : %s", name, buffer);
        if (sendData(sent, strlen(sent+1) + 1) < 0)
            return 1;
    }
    return 0;
}


void *send_func(void *data_struct) {
    SendDataFun* sendData = (SendDataFun*) data_struct;

    printf("Provide user name: ");
    if (fgets(name, BUFFER_SIZE, stdin) == NULL) {
        perror("Fail to get name");
        exit(3);
    }
    name[strlen(name) - 1] = 0;
    
    char buffer[BUFFER_SIZE];
    buffer[0] = MESSAGE;
    sprintf(buffer+1, "%s entered the chat", name);
    if (sendData(buffer, strlen(buffer+1) + 2) < 0) {
        perror("error sending name");
        exit(2);
    }

    while (1) { /* run until user enters "." to quit. */
        int size = readMessage(buffer, BUFFER_SIZE);
        if (size < 0)
            break;
        if (processAndSend(buffer, size, sendData))
            break;
    }
    return NULL;
}

static void onRecieveData(char* data, int size) {
    clearLine();
    if (data[0] == MESSAGE) {
        data[size] = 0;
        printf("%s\n", data+1);
    } else if (data[0] == FILENAME) {
        // recieve the file name
        char fileName[FILENAME_BUFFER_SIZE];
        strcpy(fileName, data+1);
        FILE* file = fopen(fileName, "w");
        if (file == NULL) {
            perror("fail to open write file");
            exit(3);
        }

        // recieving file content
        printf("Recieving file \"%s\" ", fileName);
        while(1){
            printf(".");
            fflush(stdout);
            getNextPacket(&data, &size);
            if (size < 0) {
                perror("size negative");
                exit(2);
            }
            if (data[0] == FILEEND)
                break; // recieved confirm for file transimitted
            if (data[0] != FILEDATA) {
                fprintf(stderr, "Wrong file passing prototype: %d\n", data[0]);
                exit(2);
            }
            fwrite(data+1, 1, size-1, file);
            free(data);
        }

        // end of file recieving
        printf("\n");
        fclose(file);
    } else {
        fprintf(stderr, "Unrecognized message type %d\n", data[0]);
        exit(2);
    }
    printPrompt();
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