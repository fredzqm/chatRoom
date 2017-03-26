#include "packetSocket.h"

#define BUFFER_SIZE 1024
// #define FILENAME 102
#define FILEDATA 103
#define FILEEND 104

void PacketSocket::recieve(PacketSocket* psocket) {
	char data[BUFFER_SIZE];
    while(1){
        int size = recv(psocket->sock, data, BUFFER_SIZE, 0);
        if (size <= 0) {
            perror("recv failed");
            break;
        }
    	psocket->buffer.addToBuffer(data, size);
    }
    close(psocket->sock);
}


PacketSocket::PacketSocket(int socket): sock(socket), recieveThread(recieve, this) {
}

PacketSocket::~PacketSocket() {
    this->recieveThread.detach();
}

void PacketSocket::getNextPacket(char** data, int* size) {
	this->buffer.readBuffer(data, size);
}


void PacketSocket::sendPacket(char* data, int size) {
	char buf[size+2];
    size = serializeData(buf, data, size);
    if (send(this->sock, buf, size, 0) < 0) {
    	perror("send failed");
    	exit(-1);
    }
}

void PacketSocket::sendFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "filename: %s\t", fileName);
        perror("Fail to open file");
        exit(3);
    }
    char sent[BUFFER_SIZE];
    sent[0] = FILEDATA;
    while (1) {
        int size = fread(sent+1, 1, BUFFER_SIZE-1, file);
        if (size <= 0) {
            break; // end of file
        }
        this->sendPacket(sent, size+1);
    }
    fclose(file);
    sent[0] = FILEEND;
    this->sendPacket(sent, 1);
}

void PacketSocket::recieveFile(char* fileName) {
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        perror("fail to open write file");
        exit(3);
    }
    char* data;
    int size;
    while(1){
        this->getNextPacket(&data, &size);
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
    fclose(file);
}
