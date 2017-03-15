#ifndef chatRoom_socketFactory
#define chatRoom_socketFactory

#include "io.h"

int initializeSocket(int serv_port);
int connectSocket(char* serv_name, int serv_port);

#endif