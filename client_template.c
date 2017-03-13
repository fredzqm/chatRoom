/* client.c
 * <author>
 */

#include "globals.h"

static void usage();
static void die(const char* message);
static unsigned long resolveName(const char* name);

static void initPacket(PACKET* packet, packettype control);
static void sendPacket(int sock, const PACKET* packet,
		const struct sockaddr_in* serverAddr);
static void recvPacket(int sock, PACKET* packet,
		struct sockaddr_in* serverAddr);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serverAddr;
	unsigned short serverPort;
	char *serverName;

	serverName = "localhost";
	serverPort = DEFAULTPORT;
	isVerbose = 0;

	int ch;
	while ((ch = getopt(argc, argv, "h:p:uv")) != -1) {
		switch (ch) {
		case 'h':
			serverName = optarg;
			break;
		case 'p':
			serverPort = atoi(optarg);
			break;
		case 'u':
			usage();
			break;
		case 'v':
			isVerbose = 1;
			break;
		default:
			usage();
			break;
		}
	}
	if (isVerbose)
	{
		printf("Using server %s\n", serverName);
		printf("Using port %d\n", serverPort);
	}

	//Create the socket

	//Initialize the endpoint structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_addr.s_addr = resolveName(serverName);
	serverAddr.sin_port        = htons(serverPort);

	while (1)
	{
		//Prompt for the input string

		//If no more input, break

		//Send the string to the server

	}

	close(sock);
	return 0;
}

static void usage()
{
	fprintf(stderr, "Usage: client [-u] [-v] [-h <server>] [-p <port>]\n");
	exit(1);
}

static void die(const char* message)
{
	perror(message);
	exit(1);
}

static unsigned long resolveName(const char* name)
{
	struct hostent *host;
	if ((host = gethostbyname(name)) == NULL)
		die("gethostbyname() failed");

	return *((unsigned long*)host->h_addr_list[0]);
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

