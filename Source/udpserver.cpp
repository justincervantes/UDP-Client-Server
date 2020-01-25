/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		udpserver.c -   A simple echo server using UDP
--
--	PROGRAM:			udpserver.exe
--
--	FUNCTIONS:			main
--
--	DATE:				January 23, 2020
--
--	REVISIONS:			(Date and Description)
--
--	DESIGNERS:			Justin Cervantes
--
--	PROGRAMMERS:		Justin Cervantes
--
--	NOTES:
--	The program will accept UDP packets from client machines.
--  The program will read data from the client socket and echo a handshake 
--  protocol which is described as follows:
--
--  The handshake starts with the client sending "knock knock", the server replying with
--  "dochira sama deshou ko", the client sending back "KanfyooSHas" and the server
--  responding with "o nkgx gtj o luxmkz o ykk gtj o xkskshkx o ju gtj o atjkxyzgtj"
---------------------------------------------------------------------------------------*/
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVER_UDP_PORT 		60006	// Default port
#define MAXLEN					65000	//Buffer length
#define TRUE					1

#include <stdio.h>
#include <winsock2.h>
#include <errno.h>

int main(int argc, char** argv)
{
	int	client_len, port, n;
	SOCKET sd;
	char	buf[MAXLEN];
	struct	sockaddr_in server, client;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	port = SERVER_UDP_PORT;	// Default port

	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData);

	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Can't create a socket");
		exit(1);
	}

	// Bind an address to the socket
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr*) & server, sizeof(server)) == -1)
	{
		perror("Can't bind name to socket");
		exit(1);
	}

	printf("Running UDP server...\n");

	while (TRUE)
	{
		client_len = sizeof(client);
		if ((n = recvfrom(sd, buf, MAXLEN, 0, (struct sockaddr*) & client, &client_len)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}

		//printf("Received %d bytes\t", n);
		//printf("From host: %s\n", inet_ntoa(client.sin_addr));
		//printf("Message received was: %s\n", buf);

		if (strcmp(buf, "knock knock") == 0) {

			// transmit data
			char newcode[23] = { "dochira sama deshou ko" };
			printf("Sending reply: %s\n", newcode);
			if (sendto(sd, newcode, 23, 0, (struct sockaddr*) & client, client_len) != 23)
			{
				exit(1);
			}
		}

		if (strcmp(buf, "KanfyooSHas") == 0) {
			// transmit data
			char newcode[63] = { "o nkgx gtj o luxmkz o ykk gtj o xkskshkx o ju gtj o atjkxyzgtj" };
			printf("Sending reply: %s\n", newcode);
			if (sendto(sd, newcode, 63, 0, (struct sockaddr*) & client, client_len) != 63)
			{
				exit(1);
			}
		}
	}

	closesocket(sd);
	WSACleanup();
	exit(0);
}
