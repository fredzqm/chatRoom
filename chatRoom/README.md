# Chat Room
##### This project consists of a little framework to build a chat room.

### Run the code

You can type `make` to compile all the codes.

Launch ./server and ./client, like
    
    ./client [-p portNumber] [-h hostName]
    ./server [-p portNumber]

portNumber default to be 5555. hostName default to be localhost.

You can launch multiple instance of the clients, and all of them will
appear in the same chat room.


### Functionalities

There are three types of commands you can type in a chat:

1. exit         
        to exit the chat room
2. load \<fileName\>   
        To transfer some file. This file will be transfered to everyone else
        and saved at the same relative location.
3. Any other unrecognized commands are treated as regular messages
        Other people will recieve a message indicating you said ...

### Files

#### app.c

This file handles most of the application logic of the chat room.
It specifies the specifics of prompt format. It defines the protocol
used to send data.
It defines two methods:
    
    void *send_func(SendDataFun *data_struct);
    void *recv_func(SendDataFun *data_struct);
    
These two methods define two threads that handles message sending and recieving.
They are used by the client and server driver.

send_func monitors the stdin, prints out prompt, parses user inputs, and sends corresponding message out.

recv_func monitors the socket connection, and parses and hanldes every incomming message.

#### broadCastServer.c

It provides two methods for launching a general chat room app, given a connected
socket and list of threadProcedures. defines as `` typedef void* ThreadProc(void*);`` You can provide as many threadProcedures as possible.
    
    void startServer(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
    void startClient(int sock, ThreadProc** threadls, int numThread, pthread_t* threadidls);
    
Those two methods will eastablish the connections and run the thread procedures
supplied. The pointer passed into a ThreadProc can be cast into `sendDataFun`, which
can be used to send data in the network.
    
    typedef int SendDataFun(char*, int);
    
All the messages sent by any client or server will be recieved by any others.

To retrieve a data call `void getNextPacket(char** data, int* size);`

Here is one simple example that just echo what every recieved back.
    
    void *echo_threadProc(SendDataFun *sendData) {
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
    

