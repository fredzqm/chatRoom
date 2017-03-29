#include "packetSocket.h"

#define BUFFER_SIZE 1024
// #define FILENAME 102
#define FILEDATA 103
#define FILEEND 104

void PacketSocket::receive(PacketSocket* psocket) {
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


PacketSocket::PacketSocket(int socket): sock(socket), receiveThread(receive, this) {
}

PacketSocket::~PacketSocket() {
    this->receiveThread.detach();
}

int PacketSocket::getNextPacket(char** data, int* size) {
	this->buffer.readBuffer(data, size);
    return 0;
}


int PacketSocket::sendPacket(char* data, int size) {
	char buf[size+2];
    size = serializeData(buf, data, size);
    if (send(this->sock, buf, size, 0) < 0) {
    	perror("send failed");
        return 1;
    }
    return 0;
}

int PacketSocket::sendFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        return 2;
    }
    char sent[BUFFER_SIZE];
    sent[0] = FILEDATA;
    while (1) {
        int size = fread(sent+1, 1, BUFFER_SIZE-1, file);
        if (size <= 0) {
            break; // end of file
        }
        int c;
        if ((c = this->sendPacket(sent, size+1)) != 0) {
            return c;
        }
    }
    fclose(file);
    sent[0] = FILEEND;
    this->sendPacket(sent, 1);
    return 0;
}

int PacketSocket::receiveFile(char* fileName) {
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        return 2;
    }
    char* data;
    int size;
    while(1){
        int c;
        if ((c = this->getNextPacket(&data, &size)) != 0) {
            return c;
        }
        if (size < 0) {
            perror("size negative");
            return 3;
        }
        if (data[0] == FILEEND)
            break; // received confirm for file transimitted
        if (data[0] != FILEDATA) {
            fprintf(stderr, "Wrong file passing prototype: %d\n", data[0]);
            return 4;
        }
        fwrite(data+1, 1, size-1, file);
        free(data);
    }
    fclose(file);
    return 0;
}
