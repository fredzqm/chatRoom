#include "broadCastClient.h"

void *dataReciever(void*);

#define DEFAULTPORT 5555   /* Default port for socket connection */
#define DEFAULT_SERVE_NAME "localhost"
#define IP_LENGTH 20 

int sock;

void startClient(int _sock) {
    sock = _sock;

    pthread_t pid;
    if (pthread_create(&pid, NULL, dataReciever, NULL))
        perror("Thread not created");

    char received_string[MAX_STRING_LEN];
    while(1){
        int numbytes = recv(sock, received_string, MAX_STRING_LEN, 0);
        if (numbytes <= 0)
            break;
        onRecieveBroadcast(received_string, numbytes);
    }
    close(sock);
    
    if (pthread_join(pid, NULL))
        perror("pthread_join() failed\n");
}

int csendData(char* data, int size) {
    return send(sock, data, size, 0);
}

void *dataReciever(void* arg) {
    char name[MAX_STRING_LEN];
    onStart(name, csendData);
}

