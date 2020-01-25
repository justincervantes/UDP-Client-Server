/*---------------------------------------------------------------------------------------
--	SOURCE FILE:	udpclient.c - A simple UDP client program.
--
--	PROGRAM:		udpclient.exe
--
--	FUNCTIONS:		main
--
--	DATE:			January 23, 2020
--
--	REVISIONS:		(Date and Description)
--
--	DESIGNERS:		Justin Cervantes
--
--	PROGRAMMERS:	Justin Cervantes
--
--	NOTES:
--	The program will send a UDP packet to a user specifed server, performing a
--  4-way handshake with a connected server. The handshake initiates when the client
--  connects - once it connects it starts a handshake protocol.
--
--  The handshake starts with the client sending "knock knock", the server replying with
--  "dochira sama deshou ko", the client sending back "KanfyooSHas" and the server
--  responding with "o nkgx gtj o luxmkz o ykk gtj o xkskshkx o ju gtj o atjkxyzgtj"
--
--  The server can be specified using a fully qualified domain name and or an
--	IP address. This program has hardcoded 192.168.51.142 as an IP to connect to.
--  If trying to run this program, make sure you replace this with your own ip address
--  which can be found by entering ipconfig and getting your IPv4 address in cmd line.
---------------------------------------------------------------------------------------*/
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVER_UDP_PORT		60006	// Default port
#define MAXLEN				65000   // Maximum Buffer length
#define DEFLEN				64		// Default Length

#include <stdio.h>
#include <winsock2.h>
#include <errno.h>
#include <string.h>

long delay(SYSTEMTIME t1, SYSTEMTIME t2);

int main(int argc, char** argv)
{
	int	data_size = DEFLEN, port = SERVER_UDP_PORT;
	int	i, j, server_len, client_len;
	SOCKET sd;
	char* pname, * host, rbuf[64] = { 0 }, knock[12] = { "knock knock" }, coderesponse[12] = { "KanfyooSHas" };
	struct	hostent* hp;
	struct	sockaddr_in server, client;
	SYSTEMTIME stStartTime, stEndTime;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int handshakeStage = 0;

	printf("Running UDP client...\n");


	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData);

	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Can't create a socket\n");
		exit(1);
	}

	// Store server's information
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	char hosthardcode[] = { "192.168.51.142" };
	if ((hp = gethostbyname(hosthardcode)) == NULL)
	{
		fprintf(stderr, "Can't get server's IP address\n");
		exit(1);
	}
	//strcpy((char *)&server.sin_addr, hp->h_addr);
	memcpy((char*)&server.sin_addr, hp->h_addr, hp->h_length);

	// Bind local address to the socket
	memset((char*)&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(0);  // bind to any available port
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (struct sockaddr*) & client, sizeof(client)) == -1)
	{
		perror("Can't bind name to socket");
		exit(1);
	}
	// Find out what port was assigned and print it
	client_len = sizeof(client);
	if (getsockname(sd, (struct sockaddr*) & client, &client_len) < 0)
	{
		perror("getsockname: \n");
		exit(1);
	}

	
	if (data_size > MAXLEN)
	{
		fprintf(stderr, "Data is too big\n");
		exit(1);
	}

	server_len = sizeof(server);

	// transmit data
	while (true) {

		// Send handshake initializing message
		if (handshakeStage == 0) {
			if (sendto(sd, knock, 12, 0, (struct sockaddr*) & server, server_len) == 12)
			{
				printf("Message sent: %s\n", knock);
				handshakeStage++;
			}
		}

		// Receive data and proceed through handshake
		if (recvfrom(sd, rbuf, 64, 0, (struct sockaddr*) & server, &server_len) > 0)
		{
			if (strcmp(rbuf, "dochira sama deshou ko") == 0 && handshakeStage == 1) {
				printf("Response received: %s\n", rbuf);
				handshakeStage++;
				char validationMessage[12] = { "KanfyooSHas" };
				if (sendto(sd, validationMessage, 12, 0, (struct sockaddr*) & server, server_len) == 12) {
					printf("Message sent: %s\n", validationMessage);
					handshakeStage++;
				}
			}

			if (strcmp(rbuf, "o nkgx gtj o luxmkz o ykk gtj o xkskshkx o ju gtj o atjkxyzgtj") == 0 && handshakeStage == 3) {
				printf("Response received: %s\n", rbuf);
				printf("Handshake complete!\n");
			}
		}
	}

	closesocket(sd);
	WSACleanup();
	exit(0);
}
