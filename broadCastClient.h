/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */
#ifndef chatRoom_broadCastClient
#define chatRoom_broadCastClient

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

void (*onRecieveBroadcast)(char* data, int size);

typedef int SendDataFun(char*, int);
typedef void* ThreadProc(void*);

void startClient(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);

#endif