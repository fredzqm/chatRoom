#ifndef chatRoom_buffer
#define chatRoom_buffer

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "packet.h"

#define INITIAL_SIZE 5

typedef struct {
	PACKET* packets;
	int size;
	int index;
} Buffer;


Buffer* createBuffer();

void deleteBuffer(Buffer* buffer);

void addToBuffer(Buffer* buffer, char* data, int size);

void readBuffer(Buffer* buffer, PACKET* packet);

#endif