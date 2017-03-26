#include "packetSocket.h"
#define MAX_STRING_LEN 1024

void PacketSocket::recieve(PacketSocket* psocket) {
	char data[MAX_STRING_LEN];
    while(1){
        int size = recv(psocket->sock, data, MAX_STRING_LEN, 0);
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