#include "broadCastClient.h"


void *dataReciever(void*);

#define DEFAULTPORT 5555   /* Default port for socket connection */
#define DEFAULT_SERVE_NAME "localhost"
#define IP_LENGTH 20 

void startClient(int sock) {
    pthread_t pid;
    if (pthread_create(&pid, NULL, dataReciever, &sock))
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


void *dataReciever(void* arg) {
    int sock = *((int*) arg);
    onConnectionEstablished(sock);
    pthread_exit(NULL);
}

