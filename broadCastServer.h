#ifndef chatRoom_broadCastSever
#define chatRoom_broadCastSever

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

#define MAX_STRING_LEN 1024

typedef struct {
    int index;
    pthread_t tid;
    int cid;
    void* data;
} Client;



void closeConnection(int from);

void broadcast(int from, char* data, int size);

void startServer(int sock);


void (*onRecieveBroadcast)(char* data, int size);
int (*onRecieveDataFrom)(Client* thread, char* data, int size);
void (*onAcceptConnection)(Client* thread);
void (*onCloseConnection)(Client* thread);
void (*onStart)(void* data_struct, int (*sendData)(char*, int));

#endif