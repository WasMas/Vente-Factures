#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
typedef struct
{
	int idFacture;
	int codeClient;
	char montant[256];
} Ventes;
Ventes ventesPara[256];
Ventes ventesVoitures[256];
void Entr1Config()
{
	int proxy_socket_UDP, len;
	struct sockaddr_in serverAddr_UDP, clientAddr;

	// Creating socket
	proxy_socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	serverAddr_UDP.sin_family = AF_INET;
	serverAddr_UDP.sin_addr.s_addr = INADDR_ANY;
	serverAddr_UDP.sin_port = htons(9001);

	// Binding the socket
	bind(proxy_socket_UDP, (const struct sockaddr *)&serverAddr_UDP, sizeof(serverAddr_UDP));

	len = sizeof(clientAddr);
	while (1)
	{
		recvfrom(proxy_socket_UDP, ventesPara, sizeof(ventesPara),
						 MSG_WAITALL, (struct sockaddr *)&clientAddr, &len);

		printf("Entr1 Sent Stuff:\n");
		// Processing received data (Here you can perform any operations with the received structures)
		for (int i = 0; i < 3; i++)
		{
			printf("id: %i, code client: %i, montant: %s\n", ventesPara[i].idFacture, ventesPara[i].codeClient, ventesPara[i].montant);
		}
	}
	close(proxy_socket_UDP);
}

void Entr2Config()
{
	struct sockaddr_in proxy_address_tcp;
	int proxy_socket;

	proxy_socket = socket(AF_INET, SOCK_STREAM, 0);
	proxy_address_tcp.sin_addr.s_addr = INADDR_ANY;
	proxy_address_tcp.sin_family = AF_INET;
	proxy_address_tcp.sin_port = htons(9002);

	// Listening To connection

	connect(proxy_socket, (struct sockaddr *)&proxy_address_tcp, sizeof(proxy_address_tcp));
	int ok = 1;
	send(proxy_socket, &ok, sizeof(ok), 0);
	// Receiving the array of structs
	recv(proxy_socket, ventesVoitures, sizeof(ventesVoitures), 0);
	close(proxy_socket);
}

int main()
{
	int client_socket, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int choice;

	// Creating socket file descriptor
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(9000);

	// Binding the socket to the port
	bind(client_socket, (struct sockaddr *)&address, sizeof(address));

	// Listening for incoming connections
	listen(client_socket, 50);

	// Accepting incoming connection
	new_socket = accept(client_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);

	// Receiving the integer from client
	recv(new_socket, &choice, sizeof(choice), 0);

	printf("Integer received from client: %d\n", choice);

	if (choice == 1)
	{
		Entr1Config();
	}
	else
	{
		Entr2Config();
		send(new_socket, ventesVoitures, sizeof(ventesVoitures), 0);
	}

	close(new_socket);
	close(client_socket);
	printf("Struct sent to client..\n");
	return 0;
}