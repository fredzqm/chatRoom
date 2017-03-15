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

#include "io.h"
#include "fileReader.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */

typedef struct {
    int index;
    pthread_t tid;
    int cid;
    int size;
    void* data;
    char name[MAX_STRING_LEN];
} Client;

void parseArgs(int argc, char** argv, int* port);
int initializeSocket(int serv_port);
void usage();

void broadcast(int from, char* data, int size);
void closeConnection(int from);

void *thread_func(void *arg);
void *server_func(void *arg);


void onRecieveBroadcast(char* data, int size) {
    printRecievedMessage(data);
}

int onRecieveDataFrom(Client* thread, char* data, int size) {
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


void onAcceptConnection(Client* thread) {
    if (recieveMessage(thread->cid, thread->name) < 0)
        die_with_error("failed to recieve name");
    
    char buffer[MAX_STRING_LEN];
    sprintf(buffer, "<%s> is entering the chat", thread->name);
    broadcast(thread->index, buffer, strlen(buffer));

}

void onCloseConnection(Client* thread) {
    char message[MAX_STRING_LEN];
    if (thread->index == 0) {
        sprintf(message, "<%s>(The server) closed the chat...", thread->name);
    } else {
        sprintf(message, "<%s> exits the chat...", thread->name);
    }
    broadcast(thread->index, message, strlen(message));
}


void *server_func(void *data_struct)
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



Client* ls;
int len, cap;

int main(int argc, char** argv)
{
    int serv_port = DEFAULTPORT;
    parseArgs(argc, argv, &serv_port);  /* Server port */
    int sock = initializeSocket(serv_port);

    len = 1; cap = 5;
    ls = (Client*) malloc(sizeof(Client) * cap);
    if (ls == NULL)
        die_with_error("malloc fails");

    /* This thread is responsible for handling inputs from the server */
    ls[0].index = 0;
    ls[0].cid = 0;

    /* Spawn thread */
    if (pthread_create(&ls[0].tid, NULL, server_func, (void *) ls))
        die_with_error("Thread not created");
    
    struct sockaddr addr;
    socklen_t addrlen;    
    while(1) { /* run forever */
        /* Create a client socket for an accepted connection */
        int cid = accept(sock , &addr , &addrlen );
        if (cid <= 0)
            die_with_error("accept error");

        if (len + 1 == cap) {
            cap = cap*2;
            ls = (Client*) realloc(ls, sizeof(Client) * cap);
            if (ls == NULL)
                die_with_error("malloc fails");
        }
        /* Initialize thread with id number and pointer to file descriptor */
        ls[len].index = len;
        ls[len].cid = cid;
        /* Spawn thread */
        if (pthread_create(&ls[len].tid, NULL, thread_func, (void *) &ls[len]))
            die_with_error("Thread not created");
        len++;
    }
    
    /* Check status of thread */
    while (len > 0) {
        len--;
        close(ls[len].cid);
        if (pthread_join(ls[len].tid, NULL))
            die_with_error("pthread_join() failed\n");
    }

    /* Close the welcome socket */
    fprintf(stdout, "closing server");
    free(ls);
    close(sock);
}


void closeConnection(int from) {
    Client* thread = ls + from;
    close(thread->cid);
    thread->cid = 0;
    onCloseConnection(thread);
}

void broadcast(int from, char* data, int size) {
    int i;
    for (i = 1; i < len; i++) {
        if (from != i && ls[i].cid != 0) {
            if (sendMessage(ls[i].cid, data, size) < 0) {
                closeConnection(i);
            }
        }
    }
    if (from != 0) {
        onRecieveBroadcast(data, size);
    }
}


void *thread_func(void *data_struct)
{
    Client* thread = (Client*) data_struct;
    onAcceptConnection(thread);

    char buffer[MAX_STRING_LEN];
    while(1){
        int numbytes = recieveMessage(thread->cid, buffer);
        if (numbytes <= 0)
            break;
        if (onRecieveDataFrom(ls + thread->index, buffer, numbytes))
            break;
    }
    close(thread->cid);
    pthread_exit(NULL);
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

/*
    Creates a socket. handle the error it any step fails
*/
int initializeSocket(int serv_port) {
    /* Create a TCP socket - the welcome socket */
    int sock;                         /* Socket  */
    if( (sock = socket(AF_INET , SOCK_STREAM , 0 )) < 0){
        die_with_error("socket error");
    }
  
    /* Construct local address structure */
    struct sockaddr_in serv_addr;     /* Local address */
    memset(&serv_addr, 0, sizeof(serv_addr));      /* Zero out structure */
    serv_addr.sin_family = AF_INET;                /* Internet address family */
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    serv_addr.sin_port = htons(serv_port);         /* Local port */

    /* Bind to the local address */
    if( bind(sock , (struct sockaddr*)&serv_addr , sizeof(serv_addr)) != 0)
        die_with_error("bind error");
    /* Wait for incoming requests */  
    if( listen( sock , MAX_STRING_LEN ) != 0 )
        die_with_error("listen error");
    return sock;
}

void usage() {
    fprintf(stderr, "Usage: server [-u] [-p <port>]\n");
    fprintf(stderr, "-u for usage\n");
    fprintf(stderr, "-p for server port\n");
    exit(1);
}
