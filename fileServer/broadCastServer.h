#ifndef chatRoom_broadCastSever
#define chatRoom_broadCastSever

#include "buffer.h"
#include <thread>
#include <vector>

using namespace std;

#define MAX_STRING_LEN 1024

typedef int SendDataFun(char*, int);
typedef void* ThreadProc(SendDataFun*);

void getNextPacket(char** data, int* size);


void startServer(int sock, vector<ThreadProc*>& threadls, vector<thread>& threads);
void startClient(int sock, vector<ThreadProc*>& threadls, vector<thread>& threads);

#endif