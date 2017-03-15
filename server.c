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
#include "fileReader.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */

static void parseArgs(int argc, char** argv, int* port);
static void usage();

void _onRecieveBroadcast(char* data, int size) {
    printRecievedMessage(data);
}

void* _server_func(void *data_struct)
{
    Client* thread = (Client*) data_struct;

    requestName(thread->name);
    strcpy(name, thread->name);

    while(1){
        char input_string[MAX_STRING_LEN];
        int numbytes = readMessage(input_string, MAX_STRING_LEN);
        if (numbytes < 0)
            break;
        if (onRecieveDataFrom(thread, input_string, numbytes))
            break;
    }
    exit(0);
}

int _onRecieveDataFrom(Client* thread, char* data, int size) {
    char sent[MAX_STRING_LEN];
    int isExit = strcmp("exit", data) == 0;
    if (isExit) {
        closeConnection(thread->index);
    } else {
        sprintf(sent, "<%s> : %s", thread->name, data);
        broadcast(thread->index, sent, strlen(sent));
    }
    return isExit;
}

void _onAcceptConnection(Client* thread) {
    if (recieveMessage(thread->cid, thread->name) < 0)
        die_with_error("failed to recieve name");
    
    char buffer[MAX_STRING_LEN];
    sprintf(buffer, "<%s> is entering the chat", thread->name);
    broadcast(thread->index, buffer, strlen(buffer));

}

void _onCloseConnection(Client* thread) {
    char message[MAX_STRING_LEN];
    if (thread->index == 0) {
        sprintf(message, "<%s>(The server) closed the chat...", thread->name);
    } else {
        sprintf(message, "<%s> exits the chat...", thread->name);
    }
    broadcast(thread->index, message, strlen(message));
}


int main(int argc, char** argv)
{
    int serv_port = DEFAULTPORT;
    parseArgs(argc, argv, &serv_port);  /* Server port */
    int sock = initializeSocket(serv_port);

    onRecieveBroadcast = _onRecieveBroadcast;
    onRecieveDataFrom = _onRecieveDataFrom;
    onAcceptConnection = _onAcceptConnection;
    onCloseConnection = _onCloseConnection;
    server_func = _server_func;
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

