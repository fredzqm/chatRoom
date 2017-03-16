#ifndef chatRoom_IO
#define chatRoom_IO

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

#include "broadCastServer.h"

void *send_func(void *data_struct);
void *recv_func(void *data_struct);

#endif