/**
 * @author zhangq2
 */
#include "broadCastServer.h"

#define DEFAULTPORT 5555   /* Default port for socket connection */
#define MYSELF -1

/*
 * buffer management
 */
static Buffer buffer;

static void onRecieveBroadcast(char* data, int size) {
    buffer.addToBuffer(data, size);
}

void getNextPacket(char** data, int* size) {
    buffer.readBuffer(data, size);
}


/*
 * server part
 */
typedef struct {
    int index;
    pthread_t tid;
    int cid;
} Client;


static int ssendData(char* data, int size);
static void broadcast(int from, char* data, int size);
static void *thread_func(void *data_struct);
static void closeConnection(Client* client);


static void spawnThreads(vector<ThreadProc*>& threadls, vector<thread>& threads, SendDataFun* sendData) {
    for (unsigned int i = 0; i < threadls.size(); i++) {
        threads.push_back(thread(threadls[i], sendData));
    }
}


static Client* ls;
static int len, cap;

void startServer(int sock, vector<ThreadProc*>& threadls, vector<thread>& threads) {
    // spawning threads for server
    spawnThreads(threadls, threads, ssendData);

    len = 0; cap = 5;
    ls = (Client*) malloc(sizeof(Client) * cap);
    if (ls == NULL)
        perror("malloc fails");
    
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

static int ssendData(char* data, int size) {
    char buf[size+2];
    size = serializeData(buf, data, size);
    broadcast(MYSELF, buf, size);
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
        onRecieveBroadcast(data, size);
    }
}


/*
 * client part
 */
static int csendData(char* data, int size);
static int sock;

void startClient(int _sock, vector<ThreadProc*>& threadls, vector<thread>& threads) {
    sock = _sock;

    spawnThreads(threadls, threads, csendData);

    char received_string[MAX_STRING_LEN];
    while(1){
        int numbytes = recv(sock, received_string, MAX_STRING_LEN, 0);
        if (numbytes <= 0)
            break;
        onRecieveBroadcast(received_string, numbytes);
    }
    close(sock);
}

static int csendData(char* data, int size) {
    char buf[size+2];
    size = serializeData(buf, data, size);
    return send(sock, buf, size, 0);
}


