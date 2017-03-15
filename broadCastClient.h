/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */
#ifndef chatRoom_broadCastClient
#define chatRoom_broadCastClient

#include "io.h"



void startClient(int sock);

void (*onRecieveBroadcast)(char* data, int size);
void (*onStart)(void* data_struct, int (*sendData)(char*, int));

#endif