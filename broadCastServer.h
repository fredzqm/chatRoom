#ifndef chatRoom_broadCastSever
#define chatRoom_broadCastSever

#include "io.h"

typedef struct {
    int index;
    pthread_t tid;
    int cid;
    int size;
    void* data;
    char name[MAX_STRING_LEN];
} Client;



void closeConnection(int from);

void broadcast(int from, char* data, int size);

void startServer(int sock);


void (*onRecieveBroadcast)(char* data, int size);
int (*onRecieveDataFrom)(Client* thread, char* data, int size);
void (*onAcceptConnection)(Client* thread);
void (*onCloseConnection)(Client* thread);
void* (*server_func)(void *arg);

#endif