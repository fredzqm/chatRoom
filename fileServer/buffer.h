#ifndef chatRoom_buffer
#define chatRoom_buffer

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <semaphore.h>

#define PACKET_BUFFER 1024

struct PACKET {
	int size;
	char* data;
};

class Buffer {
private:
	PACKET packets[PACKET_BUFFER];
	int start, end;
	sem_t semphore;

	char* packBuf;
	int filled;
	int nextSize;
public:
	/*
	 * create a buffer, when you are finished with it, you should use free()
	 * to release the memory
	 */
	Buffer();
	/*
	 * add a chuck of data to the buffer, they may contain one, multiple or only
	 * part of a packet
	 */
	void addToBuffer(char* data, int size);
	/*
	 * read a complete packet when it is ready
	 * *data will be assigned with a dynamically allocated memory, so make sure
	 * you free it after you use it 
	 */
	void readBuffer(char** data, int* size);
};

/*
 * serielaize the data and size format into packet format
 */
int serializeData(char* buffer, char* data, int size);


#endif