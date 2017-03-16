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
} Client;


typedef int SendDataFun(char*, int);
typedef void* ThreadProc(void*);

void startServer(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);

#endif