#include "buffer.h"

Buffer::Buffer() {
	this->packBuf = NULL;
	this->start = 0;
	this->end = 0;
	sem_init(&this->semphore, 0, 0);
	this->filled = 0;
	this->nextSize = 0;
}

void Buffer::addToBuffer(char* data, int size) {
	if (size == 0)
		return;
	if (this->packBuf == NULL) {
		if (this->nextSize != 0) { // get the second half
			this->nextSize = (data[0])       & 0x00ff | this->nextSize;   
			data+= 1; size -= 1;
		} else if (size == 1) { // only one byte for the first half of size
			this->nextSize = (data[0] << 8)  & 0xff00 ;
			return;
		} else {
			this->nextSize = (data[0] << 8)  & 0xff00 | 
						       (data[1])       & 0x00ff ;
			data+= 2; size -= 2;
		}
		this->packBuf = (char*) malloc(this->nextSize);
		if (this->packBuf == NULL) {
			perror("malloc fails");
			exit(-1);
		}
		addToBuffer(data, size);
	} else  {
		int needed = this->nextSize - this->filled;
		if (needed <= size) {
			memcpy(this->packBuf + this->filled, data, needed);
			this->packets[this->end].size = this->nextSize;
			this->packets[this->end].data = this->packBuf;
			
			this->end++;
			if (this->end == PACKET_BUFFER)
				this->end = 0;
			if (this->end == this->start) {
				perror("Buffer overflow");
				exit(0);
			}

			sem_post(&this->semphore);
			this->packBuf = NULL;
			this->nextSize = 0;
			this->filled = 0;
			addToBuffer(data + needed, size - needed);
		} else {
			memcpy(this->packBuf + this->filled, data, size);
			this->filled += size;
		}
	}
}

void Buffer::readBuffer(char** data, int* size) {
	sem_wait(&this->semphore);
	*data = this->packets[this->start].data;
	*size = this->packets[this->start].size;
	this->start++;
}


int serializeData(char* buffer, char* data, int size) {
	buffer[0] = (size >> 8) & 0xff ;
	buffer[1] = (size)      & 0xff ;
	memcpy(buffer+2, data, size);
	return size+2;
}
