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

void startClient(int sock);

void (*onRecieveBroadcast)(char* data, int size);
void (*onStart)(void* data_struct, int (*sendData)(char*, int));

#endif