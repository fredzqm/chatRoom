#include "buffer.h"

Buffer* createBuffer() {
	Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
	buf->packBuf = NULL;
	buf->start = 0;
	buf->end = 0;
	sem_init(&buf->semphore, 0, 0);
	buf->filled = 0;
	buf->nextSize = 0;
	return buf;
}

void addToBuffer(Buffer* buffer, char* data, int size) {
	if (size == 0)
		return;
	if (buffer->packBuf == NULL) {
		if (buffer->nextSize != 0) { // get the second half
			buffer->nextSize = (data[0])       & 0x00ff | buffer->nextSize;   
			data+= 1; size -= 1;
		} else if (size == 1) { // only one byte for the first half of size
			buffer->nextSize = (data[0] << 8)  & 0xff00 ;
			return;
		} else {
			buffer->nextSize = (data[0] << 8)  & 0xff00 | 
						       (data[1])       & 0x00ff ;
			data+= 2; size -= 2;
		}
		buffer->packBuf = (char*) malloc(buffer->nextSize);
		if (buffer->packBuf == NULL) {
			perror("malloc fails");
			exit(-1);
		}
		addToBuffer(buffer, data, size);
	} else  {
		int needed = buffer->nextSize - buffer->filled;
		if (needed <= size) {
			memcpy(buffer->packBuf + buffer->filled, data, needed);
			buffer->packets[buffer->end].size = buffer->nextSize;
			buffer->packets[buffer->end].data = buffer->packBuf;
			
			buffer->end++;
			if (buffer->end == PACKET_BUFFER)
				buffer->end = 0;
			if (buffer->end == buffer->start) {
				perror("Buffer overflow");
				exit(0);
			}

			sem_post(&buffer->semphore);
			buffer->packBuf = NULL;
			buffer->nextSize = 0;
			buffer->filled = 0;
			addToBuffer(buffer, data + needed, size - needed);
		} else {
			memcpy(buffer->packBuf + buffer->filled, data, size);
			buffer->filled += size;
		}
	}
}

void readBuffer(Buffer* buffer, char** data, int* size) {
	sem_wait(&buffer->semphore);
	*data = buffer->packets[buffer->start].data;
	*size = buffer->packets[buffer->start].size;
	buffer->start++;
}


int serializeData(char* buffer, char* data, int size) {
	buffer[0] = (size >> 8) & 0xff ;
	buffer[1] = (size)      & 0xff ;
	memcpy(buffer+2, data, size);
	return size+2;
}
