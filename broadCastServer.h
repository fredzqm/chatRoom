#ifndef chatRoom_broadCastSever
#define chatRoom_broadCastSever

#include "buffer.h"

#define MAX_STRING_LEN 1024

typedef int SendDataFun(char*, int);
typedef void* ThreadProc(void*);

typedef struct {
    int index;
    pthread_t tid;
    int cid;
} Client;

void getNextPacket(PACKET* packet);
void startServer(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
void startClient(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);

#endif