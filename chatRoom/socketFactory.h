#ifndef chatRoom_socketFactory
#define chatRoom_socketFactory

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

int initializeSocket(int serv_port);
int connectSocket(char* serv_name, int serv_port);

#endif