#include "socketFactory.h"

#define BACK_LOG_LENGTH 1024


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
    if( listen( sock , BACK_LOG_LENGTH ) != 0 )
        die_with_error("listen error");
    return sock;
}


int connectSocket(char* serv_name, int serv_port) {
    /* Create a TCP socket */
    int sock;                                       /* Socket  */
    if((sock = socket(AF_INET , SOCK_STREAM , 0 ) ) < 0)
        die_with_error("socket error");

    /* parse the host name */
    struct hostent *host;
    if ((host=gethostbyname(serv_name)) == NULL)
        die_with_error("gethostbyname() failed");
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

