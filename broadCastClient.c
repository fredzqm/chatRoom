#include "broadCastClient.h"


void *dataReciever(void*);

#define DEFAULTPORT 5555   /* Default port for socket connection */
#define DEFAULT_SERVE_NAME "localhost"
#define IP_LENGTH 20 

typedef struct {
    int sock;
    void (*onConnectionEstablished)(int);
} DataRecieverInfo;

void startClient(int sock, 
    void (*onRecieveBroadcast)(char*, int),
    void (*onConnectionEstablished)(int) ) {

    DataRecieverInfo info;
    info.sock = sock;
    info.onConnectionEstablished = onConnectionEstablished;

    pthread_t pid;
    if (pthread_create(&pid, NULL, dataReciever, &info))
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
    DataRecieverInfo* info = (DataRecieverInfo*)arg;
    info->onConnectionEstablished(info->sock);
    pthread_exit(NULL);
}

int connectSocket(char* serv_name, int serv_port, char* ip) {
    /* Create a TCP socket */
    int sock;                                       /* Socket  */
    if((sock = socket(AF_INET , SOCK_STREAM , 0 ) ) < 0)
        die_with_error("socket error");

    /* parse the host name */
    struct hostent *host;
    if ((host=gethostbyname(serv_name)) == NULL)
        die_with_error("gethostbyname() failed");
    struct in_addr ** addr_list = (struct in_addr **) host->h_addr_list;
    strcpy(ip , inet_ntoa(*addr_list[0]));
    unsigned long s_addr = *((unsigned long *)host->h_addr_list[0]);

    /* Construct local address structure */
    struct sockaddr_in serv_addr;                   /* Server address */
    memset(&serv_addr, 0, sizeof(serv_addr));       /* Zero out structure */
    serv_addr.sin_family = AF_INET;                 /* Internet address family */
    serv_addr.sin_addr.s_addr = s_addr; /* Server address */
    serv_addr.sin_port = htons(serv_port);          /* Local port */

    /* Connect to server socket */
    if (connect(sock , (struct sockaddr*) &serv_addr , sizeof(serv_addr) ) != 0)
        die_with_error("connect error");
    return sock;
}

