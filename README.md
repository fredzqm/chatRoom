#####################################################################
# Source Files
######################################################################

This directory contains the files you will need for lab09
app.c
    This file handles most of the application logic of the chat room.
    It specifies the specifics of prompt format. It defines the protocol
    used to send data.
    It defines two methods:
    ```
        void *send_func(void *data_struct);
        void *recv_func(void *data_struct);
    ```
    
    These two methods define how the application, and used by the client
    and server driver.

broadCastServer.c
    It provides two method 
    ```
        void startServer(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
        void startClient(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
    ```

    
buffer.c
    This is buffer built for TCP that break the stream into packets, so
    the reciever the same number of packets as the sender sends it.

server.c
    The driver for server, basically launch the app with the two methods
    defined in app.c
    
client.c
    The driver for client, basically launch the app with the two methods
    defined in app.c
    
###############################################
# You shouldn't modify any of these files below
###############################################

Makefile:
    Run make to create executables.

