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
        die_with_error("Thread not created");

    char received_string[MAX_STRING_LEN];
    while(1){
        int numbytes = recieveMessage(sock, received_string);
        if (numbytes <= 0)
            break;
        onRecieveBroadcast(received_string, numbytes);
    }
    close(sock);
    
    if (pthread_join(pid, NULL))
        die_with_error("pthread_join() failed\n");
}

int csendData(char* data, int size) {
    return sendMessage(sock, data, size);
}

void *dataReciever(void* arg) {
    onStart(name, csendData);
    pthread_exit(NULL);
}

