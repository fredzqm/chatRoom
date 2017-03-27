#include "buffer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

using namespace std;

class PacketSocket {
private:
	Buffer buffer;
	int sock;
	thread receiveThread;
	static void receive(PacketSocket* psocket);
public:
	PacketSocket(int socket);
	int getNextPacket(char** data, int* size);
	int receiveFile(char* fileName);
	int sendFile(char* fileName);
	int sendPacket(char* data, int size);
	~PacketSocket();
};
