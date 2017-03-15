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

int readMessage(char* buffer, int maxSize);
void requestName(char* buffer);
void printRecievedMessage(char* message);

char name[MAX_STRING_LEN];

#endif