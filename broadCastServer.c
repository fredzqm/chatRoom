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
#define MYSELF -1

static int sendData(char* data, int size);
static void broadcast(int from, char* data, int size);
static void *thread_func(void *data_struct);
static void closeConnection(Client* client);

Client* ls;
int len, cap;


void startServer(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls) {
    len = 0; cap = 5;
    ls = (Client*) malloc(sizeof(Client) * cap);
    if (ls == NULL)
        perror("malloc fails");

    // spawning threads for server
    pthread_t temp[numThread];
    if (threadidls == NULL)
        threadidls = temp;
    for (int i = 0; i < numThread; i++) {
        if (pthread_create(&threadidls[i], NULL, threadls[i], sendData)) {
            perror("Thread not created");
        }
    }
    
    // waiting for connections and establish a thread to recieve message from each.
    struct sockaddr addr;
    socklen_t addrlen;    
    while(1) { /* run forever */
        /* Create a client socket for an accepted connection */
        int cid = accept(sock , &addr , &addrlen );
        if (cid <= 0)
            perror("accept error");

        if (len + 1 == cap) {
            cap = cap*2;
            ls = (Client*) realloc(ls, sizeof(Client) * cap);
            if (ls == NULL)
                perror("malloc fails");
        }
        /* Initialize thread with id number and pointer to file descriptor */
        ls[len].index = len;
        ls[len].cid = cid;
        /* Spawn thread */
        if (pthread_create(&ls[len].tid, NULL, thread_func, (void *) &ls[len]))
            perror("Thread not created");
        len++;
    }
    
    /* Check status of thread */
    while (len > 0) {
        len--;
        close(ls[len].cid);
        if (pthread_join(ls[len].tid, NULL))
            perror("pthread_join() failed\n");
    }

    /* Close the welcome socket */
    fprintf(stdout, "closing server");
    free(ls);
    close(sock);
}

static void *thread_func(void *data_struct) {
    Client* client = (Client*) data_struct;
    char buffer[MAX_STRING_LEN];
    while(1){
        int numbytes = recv(client->cid, buffer, MAX_STRING_LEN, 0);
        if (numbytes <= 0)
            break;
        broadcast(client->index, buffer, numbytes);
    }
    closeConnection(client);
    return NULL;
}

static void closeConnection(Client* client) {
    close(client->cid);
    client->cid = 0;
}

static int sendData(char* data, int size) {
    broadcast(MYSELF, data, size);
    return size;
}

static void broadcast(int from, char* data, int size) {
    int i;
    for (i = 0; i < len; i++) {
        if (from != i && ls[i].cid != 0) {
            if (send(ls[i].cid, data, size, 0) < 0) {
                closeConnection(ls + i);
            }
        }
    }
    if (from != MYSELF) {
        // onRecieveBroadcast(data, size);
    }
}



