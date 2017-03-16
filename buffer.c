#include "buffer.h"

Buffer* createBuffer() {
	Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
	sem_init(&buf->semphore, 0, 0);
	buf->start = 0;
	buf->end = 0;
	buf->packBuf = NULL;
	return buf;
}

void addToBuffer(Buffer* buffer, char* data, int size) {
	if (buffer->packBuf == NULL) {
		long packSize = (data[0] << 8)  & 0xff00 | 
						(data[1])       & 0x00ff ;   
		buffer->packBuf = (char*) malloc(packSize);
		if (buffer->packBuf == NULL) {
			perror("malloc fails");
			exit(-1);
		}
		buffer->filled = 0;
		buffer->nextSize = packSize;
		sem_post(&buffer->semphore);
		addToBuffer(buffer, data+2, size-2);
	} else  {
		int needed = buffer->nextSize - buffer->filled;
		if (needed <= size) {
			memcpy(buffer->packBuf + buffer->filled, data, needed);
			buffer->end++;
			if (buffer->end == PACKET_BUFFER)
				buffer->end = 0;
			if (buffer->end == buffer->start) {
				perror("Buffer overflow");
				exit(0);
			}
			buffer->packets[buffer->end].size = buffer->nextSize;
			buffer->packets[buffer->end].data = buffer->packBuf;

			buffer->packBuf = NULL;
			addToBuffer(buffer, data + needed, size - needed);
		} else {
			memcpy(buffer->packBuf + buffer->filled, data, size);
			buffer->filled += size;
			buffer->nextSize -= size;
		}
	}
}

void readBuffer(Buffer* buffer, char** data, int* size) {
	sem_wait(&buffer->semphore);
	*data = buffer->packets[buffer->start].data;
	*size = buffer->packets[buffer->start].size;
	buffer->start++;
}


