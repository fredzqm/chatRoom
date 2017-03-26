/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */

#include "packetSocket.h"
#include "socketFactory.h"
#include "flags.h"

#define BUFFER_SIZE 1024

static void usage();
static void parseArgs(int argc, char** argv, char** hostName, int* port);


int main(int argc, char *argv[]) {
    int serv_port = DEFAULTPORT;                         /* Server port */
    char* serv_name = DEFAULT_SERVE_NAME;                /* Server host name */
    
    /* Parse command line arguments */
    parseArgs(argc, argv, &serv_name, &serv_port);
    int sock = connectSocket(serv_name, serv_port);

    PacketSocket psocket(sock);
    // char* data;
    // int size;
    while (1) {
        char buffer[BUFFER_SIZE];
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("reading input failed");
            exit(3);
        }
        int len = strlen(buffer)-1;
        buffer[len] = 0;
        if (strcmp(buffer, "exit") == 0) {
            buffer[0] = END;
            psocket.sendPacket(buffer, 1);
            exit(0);
        } else if (strncmp(buffer, "iWant ", 6) == 0) {
            buffer[5] = WANT;
            psocket.sendPacket(buffer+5, len-5);
            psocket.recieveFile("recievedFile");
        } else if (strncmp(buffer, "uTake ", 6) == 0) {
            buffer[5] = TAKE;
            psocket.sendPacket(buffer+5, len-5);
            psocket.sendFile(buffer+6);
        }
    }
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



