#include "buffer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

using namespace std;

class PacketSocket {
private:
	Buffer buffer;
	int sock;
	thread recieveThread;
	static void recieve(PacketSocket* psocket);
public:
	PacketSocket(int socket);
	void getNextPacket(char** data, int* size);
	void recieveFile(char* fileName);
	void sendFile(char* fileName);
	void sendPacket(char* data, int size);
	~PacketSocket();
};
