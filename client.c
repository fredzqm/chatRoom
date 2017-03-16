/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */

#include "broadCastServer.h"
#include "socketFactory.h"
#include "app.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */
#define DEFAULT_SERVE_NAME "localhost"
#define IP_LENGTH 20 

static void usage();
static void parseArgs(int argc, char** argv, char** hostName, int* port);

void *dataReciever(void* arg) {
    SendDataFun* sendData = (SendDataFun*) arg;
    
    char name[MAX_STRING_LEN];
    requestName(name);

    if (sendData(name, strlen(name)) < 0)
        perror("error sending name");

    char buffer[MAX_STRING_LEN];
    while (1) { /* run until user enters "." to quit. */
        int numbytes = readMessage(buffer, MAX_STRING_LEN);
        if (numbytes < 0)
            break;
        if (processAndSend(buffer, numbytes, sendData) < 0)
            break;
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    int serv_port = DEFAULTPORT;                           /* Server port */
    char* serv_name = DEFAULT_SERVE_NAME;                /* Server host name */
    
    /* Parse command line arguments */
    parseArgs(argc, argv, &serv_name, &serv_port);
    int sock = connectSocket(serv_name, serv_port);

    ThreadProc* threads[] = {dataReciever};
    startClient(sock, threads, 1, NULL);
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



