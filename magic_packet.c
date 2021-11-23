// credit to https://github.com/GramThanos/WakeOnLAN

#ifdef __linux
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#endif
#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#endif
#ifdef __APPLE__
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "header.h"

// Create Magic Packet
void createMagicPacket(unsigned char packet[], unsigned int macAddress[]){
	int i;
	// Mac Address Variable
	unsigned char mac[6];

	// 6 x 0xFF on start of packet
	for(i = 0; i < 6; i++){
		packet[i] = 0xFF;
		mac[i] = macAddress[i];
	}
	// Rest of the packet is MAC address of the pc
	for(i = 1; i <= 16; i++){
		memcpy(&packet[i * 6], &mac, 6 * sizeof(unsigned char));
	}
}

// Main Program
int send_magic_packet(){
	// Default broadcast address
	char broadcastAddress[] = "255.255.255.255";
	// Packet buffer    
	unsigned char packet[102];
	// Mac address
	unsigned int mac[6];
	// Set broadcast
	int broadcast = 1;

	// Windows socket
	#ifdef _WIN32
		// Socket data
		WSADATA data;
		// Socket
		SOCKET udpSocket;
	#endif

	// Socket address
	struct sockaddr_in udpClient, udpServer;
	
	sscanf("00:11:22:33:44:55","%x:%x:%x:%x:%x:%x", &(mac[0]), &(mac[1]), &(mac[2]), &(mac[3]), &(mac[4]), &(mac[5]));


	// Create Magic Packet
	createMagicPacket(packet, mac);
    printf("Packet will be sent to %02hhx:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf(" via the broadcast address %s.\n", broadcastAddress);

	// MacOS and Linux
	#if defined(__APPLE__) || defined(__linux)
		int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (udpSocket == -1) {
			printf("An error was encountered creating the UDP socket: '%s'.\n", strerror(errno));
			return (EXIT_FAILURE);
		}
		int setsock_result = setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
		if (setsock_result == -1) {
			printf("Failed to set socket options: '%s'.\n", strerror(errno));
			return(EXIT_FAILURE);
		}
		// Set parameters
		udpClient.sin_family = AF_INET;
		udpClient.sin_addr.s_addr = INADDR_ANY;
		udpClient.sin_port = 0;
		// Bind socket
		int bind_result = bind(udpSocket, (struct sockaddr*) &udpClient, sizeof(udpClient));
		if (bind_result == -1) {
			printf("Failed to bind socket: '%s'.\n", strerror(errno));
			return(EXIT_FAILURE);
		}

		// Set server end point (the broadcast addres)
		udpServer.sin_family = AF_INET;
		udpServer.sin_addr.s_addr = inet_addr(broadcastAddress);
		udpServer.sin_port = htons(9);

		// Send the packet
		int result = sendto(udpSocket, &packet, sizeof(unsigned char) * 102, 0, (struct sockaddr*) &udpServer, sizeof(udpServer));
		if (result == -1) {
			printf("Failed to send magic packet to socket: '%s'.\n", strerror(errno));
			return(EXIT_FAILURE);
		}
	#endif


	// Windows socket
	#ifdef _WIN32
		// Setup broadcast socket
		WSAStartup(MAKEWORD(2, 2), &data);
		udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char *) &broadcast, sizeof(broadcast)) == -1){
			printf("Failed to setup a broadcast socket.\n");
			return(EXIT_FAILURE);
		}
		// Set parameters
		udpClient.sin_family = AF_INET;
		udpClient.sin_addr.s_addr = INADDR_ANY;
		udpClient.sin_port = htons(0);
		// Bind socket
		bind(udpSocket, (struct sockaddr*) &udpClient, sizeof(udpClient));

		// Set server end point (the broadcast addres)
		udpServer.sin_family = AF_INET;
		udpServer.sin_addr.s_addr = inet_addr(broadcastAddress);
		udpServer.sin_port = htons(9);

		// Send the packet
		sendto(udpSocket, (char*)&packet, sizeof(unsigned char) * 102, 0, (struct sockaddr*) &udpServer, sizeof(udpServer));
	#endif

	// Done
	printf("Wake up packet was sent.\n");
	return(EXIT_SUCCESS);
}
