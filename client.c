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

#include "broadCastClient.h"
#include "fileReader.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */
#define DEFAULT_SERVE_NAME "localhost"
#define IP_LENGTH 20 

static int connectSocket(char* serv_name, int serv_port, char* ip);
static void usage();
static void parseArgs(int argc, char** argv, char** hostName, int* port);

void _onRecieveBroadcast(char* data, int size) {
    printRecievedMessage(data);
}

void _onConnectionEstablished(int sock) {
    requestName(name);
    if (sendMessage(sock, name, strlen(name)) < 0)
        die_with_error("error sending name");

    char input_string[MAX_STRING_LEN];
    while (1) { /* run until user enters "." to quit. */
        int numbytes = readMessage(input_string, MAX_STRING_LEN);
        if (numbytes < 0)
            break;
        if (sendMessage(sock, input_string, numbytes) < 0)
            break;
    }
}


int main(int argc, char *argv[]) {
    int serv_port = DEFAULTPORT;                           /* Server port */
    char* serv_name = DEFAULT_SERVE_NAME;                  /* Server host name */
    char ip[IP_LENGTH];
    
    /* Parse command line arguments */
    parseArgs(argc, argv, &serv_name, &serv_port);
    int sock = connectSocket(serv_name, serv_port, ip);
    printf("Connection established with %s\n", ip);

    onRecieveBroadcast = _onRecieveBroadcast;
    onConnectionEstablished = _onConnectionEstablished;
    startClient(sock);
}

void parseArgs(int argc, char** argv, char** hostName, int* port) {
    int ch;
    while ((ch=getopt(argc, argv, "h:p:u")) != -1) {
        switch (ch) {
            case 'h':
                *hostName = optarg;
                printf("Using server %s\n", *hostName);
                break;
            case 'p':
                *port = atoi(optarg);
                printf("Using port %d\n", *port);
                break;
            case 'u':
            default:
                usage();
                break;
        }
    }
}

/* usage - print description of command arguments */
void usage() {
    fprintf(stderr, "Usage: client [-u] [-v] -h <server> [-p <port>]\n");
    fprintf(stderr, "-u for usage\n");
    fprintf(stderr, "-v for verbose mode\n");
    fprintf(stderr, "-h for server name\n");
    fprintf(stderr, "-p for server port\n");
    exit(1);
}


int connectSocket(char* serv_name, int serv_port, char* ip) {
    /* Create a TCP socket */
    int sock;                                       /* Socket  */
    if((sock = socket(AF_INET , SOCK_STREAM , 0 ) ) < 0)
        die_with_error("socket error");

    /* parse the host name */
    struct hostent *host;
    if ((host=gethostbyname(serv_name)) == NULL)
        die_with_error("gethostbyname() failed");
    struct in_addr ** addr_list = (struct in_addr **) host->h_addr_list;
    strcpy(ip , inet_ntoa(*addr_list[0]));
    unsigned long s_addr = *((unsigned long *)host->h_addr_list[0]);

    /* Construct local address structure */
    struct sockaddr_in serv_addr;                   /* Server address */
    memset(&serv_addr, 0, sizeof(serv_addr));       /* Zero out structure */
    serv_addr.sin_family = AF_INET;                 /* Internet address family */
    serv_addr.sin_addr.s_addr = s_addr; /* Server address */
    serv_addr.sin_port = htons(serv_port);          /* Local port */

    /* Connect to server socket */
    if (connect(sock , (struct sockaddr*) &serv_addr , sizeof(serv_addr) ) != 0)
        die_with_error("connect error");
    return sock;
}

