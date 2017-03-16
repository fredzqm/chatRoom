# Chat Room
##### This project consists of a little framework to build a chat room.

### Functionalities

You can type make to compile all the codes.

Launch ./client and ./server.

There are three types of commands you can type:

1. exit         
        to exit the chat room
2. loat \<fileName\>   
        to transfer some file
3. Any other unrecognized commands are treated as regular messages

### Files

#### app.c

This file handles most of the application logic of the chat room.
It specifies the specifics of prompt format. It defines the protocol
used to send data.
It defines two methods:
    
    void *send_func(void *data_struct);
    void *recv_func(void *data_struct);
    
These two methods define how the application, and used by the client
and server driver.

#### broadCastServer.c

It provides two methods for launching a general chat room app, given a connected
socket and list of threadProcedures. defines as `` typedef void* ThreadProc(void*);``
    
    void startServer(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
    void startClient(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
    
Those two methods will eastablish the connections and run the thread procedures
supplied. The pointer passed into a ThreadProc can be cast into `sendDataFun`, which
can be used to send data in the network.
    
    typedef int SendDataFun(char*, int);
    
All the messages sent by any client or server will be recieved by any others.

To retrieve a data call `void getNextPacket(char** data, int* size);`

Here is one simple example that just echo what every recieved back.
    
    void *echo_threadProc(void *data_struct) {
        SendDataFun* sendData = (SendDataFun*) data_struct;
        char* data;
        int size;
        while(1){
            getNextPacket(&data, &size);
            sendData(data, size);
            free(data);
        }
    }
    
#### buffer.c

This is buffer built for TCP that break the stream into packets, so the reciever
the same number of packets as the sender sends it.

#### server.c

The driver for server, basically launch the app with the two methods defined in app.c
    
#### client.c

The driver for client, basically launch the app with the two methods defined in app.c
    

