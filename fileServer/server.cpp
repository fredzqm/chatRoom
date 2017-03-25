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


#define DEFAULTPORT 5555   /* Default port for socket connection */

static void parseArgs(int argc, char** argv, int* port);
static void usage();
static int isCloseRequest(char* request, int size);
static int isDownloadRequest(char* request, int size);
static int isUploadRequest(char* request, int size);
static void handleDownloadRequest(char* request, PacketSocket* psockPtr, int size);
static void handleUploadRequest(char* request, PacketSocket* psockPtr, int size);
static void handleEachThread(const std::thread connection);

void onConnect(int sock) {
    PacketSocket psocket(sock);
    while (1) {
        char* data;
        int size;
        psocket.getNextPacket(&data, &size);
        // Check request type:
        // 1. ";;;" = exit
        // 2. "iWant" = download
        // 3. "uTake" = upload
        // 4. Malformed request
        if (isCloseRequest(data, size)) {
            printf("Taken");
            close(sock);
            delete data;
            break;
        } else if (isDownloadRequest(data, size)) {
            // handleDownloadRequest(data, &psocket, size);
        } else if (isUploadRequest(data, size)) {
            // handleUploadRequest(data, &psocket, size);
        } else {
            psocket.sendPacket("Wrong request format!\n", 22);
        }
        delete data;
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

int isCloseRequest(char* request, int size) {
    return size == 3 && request[0] == ';' && request[1] == ';' && request[2] == ';';
}

int isDownloadRequest(char* request, int size) {
    return size > 6 && request[0] == 'i' && request[1] == 'W' && request[2] == 'a' && request[3] == 'n' && request[4] == 't';
}

int isUploadRequest(char* request, int size) {
    return size > 6 && request[0] == 'u' && request[1] == 'T' && request[2] == 'a' && request[3] == 'k' && request[4] == 'e';
}
