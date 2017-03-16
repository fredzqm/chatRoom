/* globals.c
 * Kevin Wells, Spring 2012
 */

#include "globals.h"

int isVerbose = 0;

static bool validateChecksum(const PACKET* packet);

static void hexdump(const unsigned char* data, size_t length);

const char* getPacketType(const PACKET* packet)
{
	switch (packet->control)
	{
	case ACK:
		return "ACK";
	case DATA:
		return "DATA";
	case ECHO:
		return "ECHO";
	case GET:
		return "GET";
	case PUT:
		return "PUT";
	case RETR:
		return "RETR";
	default:
		return "UNKNOWN";
	}
}

void printPacket(const PACKET* packet)
{
	if (packet->control < 0 || packet->control > RETR)
		printf("Control:  \033[1;31mUNKNOWN\033[0m\n");
	else
		printf("Control:  \033[0;32m%s\033[0m\n", getPacketType(packet));

	if (!validateChecksum(packet))
		printf("Checksum: \033[1;31m%08X\033[0m\n", packet->checksum);
	else
		printf("Checksum: \033[0;32m%08X\033[0m\n", packet->checksum);

	if (packet->loadsize < 0 || packet->loadsize > LOADSIZE)
		printf("Loadsize: \033[1;31m%d\033[0m\n", packet->loadsize);
	else
		printf("Loadsize: \033[0;32m%d\033[0m\n", packet->loadsize);

	if (packet->sequence < 0)
		printf("Sequence: \033[0;31m%d\033[0m\n", packet->sequence);
	else
		printf("Sequence: \033[0m%d\033[0m\n", packet->sequence);

	if (isVerbose)
	{
		char data[LOADSIZE+3] = {0,};
		memcpy(data, packet->data, LOADSIZE);
		char* n = strtok(data, "\r\n"); 
		if ((n = strtok(NULL, "\r\n")))
			strcpy(n-1, "...");
		printf("Data:     %s\n", data);
		int load = packet->loadsize;
		if (load > LOADSIZE)
			load = LOADSIZE;
		hexdump((uint8_t*)packet->data, load);
	}
	printf("\n");
}

static bool validateChecksum(const PACKET* packet)
{
	//TODO Add checksum validation
	return true;
}

static void hexdump(const unsigned char* data, size_t length)
{
	int i;
	size_t stoplength = (length % 16 == 0) ? length : (length/16+1)*16;
	for (i = 0; i < stoplength; i++)
	{
		if (i < length)
			printf("%02X ", (unsigned char)data[i]);
		else
			printf("   ");

		if ((i+1) % 16 == 0)
		{
			printf("  ");
			int x;
			for (x = 0; x < ((i < length) ? 16 : (length % 16)); x++)
			{
				char c = data[i-15+x];
				if (isprint(c))
					printf("%c", c);
				else
					printf(".");
			}
			printf("\n");
		}
	}
}


static void initPacket(PACKET* packet, packettype control)
{
    //TODO Initialize packet structure to a sane state
}

static void sendPacket(int sock, const PACKET* packet,
        const struct sockaddr_in* serverAddr)
{
    //TODO Send packet
}

static void recvPacket(int sock, PACKET* packet,
        struct sockaddr_in* serverAddr)
{
    //TODO Receive packet
}

