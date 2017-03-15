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
#include "fileReader.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */

static void *thread_func(void *arg);
static void *server_func(void *arg);

Client* ls;
int len, cap;

void startServer(int sock) {
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

