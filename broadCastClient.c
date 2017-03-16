#include "broadCastClient.h"

static int sendData(char* data, int size);
static int sock;

void startClient(int _sock, ThreadProc** threadls, int numThread, pthread_t* threadidls) {
    sock = _sock;
    // spawning threads for client
    pthread_t temp[numThread];
    if (threadidls == NULL)
        threadidls = temp;
    for (int i = 0; i < numThread; i++) {
        if (pthread_create(&threadidls[i], NULL, threadls[i], sendData)) {
            perror("Thread not created");
        }
    }

    char received_string[MAX_STRING_LEN];
    while(1){
        int numbytes = recv(sock, received_string, MAX_STRING_LEN, 0);
        if (numbytes <= 0)
            break;
        onRecieveBroadcast(received_string, numbytes);
    }
    close(sock);
}

static int sendData(char* data, int size) {
    return send(sock, data, size, 0);
}

