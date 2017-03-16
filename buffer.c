#include "buffer.h"

// typedef struct {
// 	PACKET* packets;
// 	int size;
// 	int index;
// 	char* buffer;
// 	int bufferSize;
// } Buffer;


Buffer* createBuffer() {
	Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
	buf->size = INITIAL_SIZE;
	buf->index = 0;
	buf->packets = (PACKET*) malloc(INITIAL_SIZE * sizeof(PACKET));
	return buf;
}

void deleteBuffer(Buffer* buffer) {
	
}

void addToBuffer(Buffer* buffer, char* data, int size) {

}

void readBuffer(Buffer* buffer, PACKET* packet) {

}


