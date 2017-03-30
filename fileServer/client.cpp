/**
 * Lab09 Solution File - simple client for echo server
 * @author Fred Zhang
 */

#include "packetSocket.h"
#include "socketFactory.h"
#include "flags.h"
#include <iostream>

static void usage();
static void parseArgs(int argc, char** argv, char** hostName, int* port);


int getLine(char* buffer) {
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        perror("reading input failed");
        exit(3);
    }
    int len = strlen(buffer)-1;
    buffer[len] = 0;
    return len;
}

int main(int argc, char *argv[]) {
    int serv_port = DEFAULTPORT;                         /* Server port */
    char* serv_name = DEFAULT_SERVE_NAME;                /* Server host name */
    
    /* Parse command line arguments */
    parseArgs(argc, argv, &serv_name, &serv_port);
    int sock = connectSocket(serv_name, serv_port);

    PacketSocket psocket(sock);
    while (1) {
        char buffer[BUFFER_SIZE];
        cout << ">> " << flush;
        int len = getLine(buffer);
        if (strcmp(buffer, "exit") == 0) {
            buffer[0] = END;
            psocket.sendPacket(buffer, 1);
            exit(0);
        } else if (strncmp(buffer, "iWant ", 6) == 0) {
            buffer[5] = WANT;
            psocket.sendPacket(buffer+5, len-5);
            char* data;
            int size;
            psocket.getNextPacket(&data, &size);
            if (data[0] == ERROR) {
                cout << "The file is not found." << endl;
            } else {
                cout << "Where do you want to save this? " << flush;
                getLine(buffer);
                psocket.receiveFile(buffer);
            }
            delete data;
        } else if (strncmp(buffer, "uTake ", 6) == 0) {
            FILE* file = fopen(buffer+6, "r");
            if (file == NULL) {
                cout << "file " << string(buffer+6) << " does not exists" << endl;
            } else {
                fclose(file);
                char take[BUFFER_SIZE];
                take[0] = TAKE;
                cout << "Where do you want to put this? " << flush;
                getLine(take+1);
                psocket.sendPacket(take, 2+strlen(take+1));
                psocket.sendFile(buffer+6);
            }
        } else {
            cout << "Unknow command" << endl;
            // psocket.sendPacket(buffer, len);
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



