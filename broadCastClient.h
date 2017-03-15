/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */
#ifndef chatRoom_broadCastClient
#define chatRoom_broadCastClient

#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include "io.h"



void startClient(int sock);

void (*onRecieveBroadcast)(char* data, int size);
void (*onConnectionEstablished)(int sock);

#endif