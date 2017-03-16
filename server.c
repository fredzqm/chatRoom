/**
 * @author zhangq2
 */
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "broadCastServer.h"
#include "socketFactory.h"
#include "app.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */

static void parseArgs(int argc, char** argv, int* port);
static void usage();

void _onStart(void* data_struct, int (*sendData)(char*, int)) {
    char* _name = (char*) data_struct;

    requestName(name);
    strcpy(_name, name);

    while(1){
        char buffer[MAX_STRING_LEN];
        int numbytes = readMessage(buffer, MAX_STRING_LEN);
        if (numbytes < 0)
            break;
        if (processAndSend(buffer, numbytes, sendData) < 0)
            break;
    }
    exit(0);
}


int main(int argc, char** argv)
{
    int serv_port = DEFAULTPORT;
    parseArgs(argc, argv, &serv_port);  /* Server port */
    int sock = initializeSocket(serv_port);

    onRecieveBroadcast = _onRecieveBroadcast;
    onStart = _onStart;
    startServer(sock);
}


/*
    initialize serv_port based on arguments
*/
void parseArgs(int argc, char** argv, int* port) {
    int optch;                        /* option flag */
    if (argc < 1) {
        usage();
    }
    while ((optch = getopt(argc, argv, "p:u")) != -1) {
        switch (optch) {
        case 'p':
            *port = atoi(optarg);
            printf("Using port %d\n", *port);
            break;
        case 'u':
        default:
            usage();
        }
    }
}


void usage() {
    fprintf(stderr, "Usage: server [-u] [-p <port>]\n");
    fprintf(stderr, "-u for usage\n");
    fprintf(stderr, "-p for server port\n");
    exit(1);
}

