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
#include <vector>

#include "packetSocket.h"
#include "socketFactory.h"
#include "flags.h"

static void parseArgs(int argc, char** argv, int* port);
static void usage();

void onConnect(int sock) {
    PacketSocket psocket(sock);
    while (1) {
        char* data;
        int size;
        psocket.getNextPacket(&data, &size);
        switch (data[0]) {
            case END:
                delete data;
                return;
            case WANT:
                printf("want: %s\n", data+1);
                char actualFilePath[BUFFER_SIZE] = "./serverstore/";
                strncat(actualFilePath, data+1, size-1);
                psocket.sendFile(actualFilePath);
                break;
            case TAKE:
                char actualFilePath[BUFFER_SIZE] = "./serverreceived/";
                strncat(actualFilePath, data+1, size-1);
                psocket.receiveFile(actualFilePath);
                break;
            default:
                fprintf(stderr, "Wrong request format: %d + %s\n", data[0], data+1);
                exit(10);
        }
    }
}

int main(int argc, char** argv)
{
    int serv_port = DEFAULTPORT;
    parseArgs(argc, argv, &serv_port);  /* Server port */
    int sock = initializeSocket(serv_port);

    
    vector<thread> connections;
    struct sockaddr addr;
    socklen_t addrlen;    
    while(1) { /* run forever */
        /* Create a client socket for an accepted connection */
        int cid = accept(sock , &addr , &addrlen );
        if (cid <= 0) {
            perror("accept error");
            exit(3);
        }
        connections.push_back(thread(onConnect, cid));
    }
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
