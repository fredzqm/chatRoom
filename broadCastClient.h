/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */
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
#include <arpa/inet.h>

#include "io.h"


int connectSocket(char* serv_name, int serv_port, char* ip);

void startClient(int sock, 
	void (*onRecieveBroadcast)(char* data, int size),
	void (*onConnectionEstablished)(int sock)
	);
