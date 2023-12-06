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
	struct sockaddr_in serverAddr;
	int sockfd, n;
	int inputInt = 10; // Input integer to send to the server

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9001);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	n = sendto(sockfd, &inputInt, sizeof(inputInt), 0, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (n < 0)
	{
		perror("Send to server failed");
		exit(EXIT_FAILURE);
	}

	printf("Integer sent to server: %d\n", inputInt);

	n = recvfrom(sockfd, &ventesPara, sizeof(ventesPara), 0, NULL, NULL);
	if (n < 0)
	{
		perror("Receive from server failed");
		exit(EXIT_FAILURE);
	}

	printf("Received array of structs from server:\n");

	close(sockfd);
}

void Entr2Config()
{
	struct sockaddr_in proxy_address_tcp;
	int proxy_socket;

	proxy_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (proxy_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	proxy_address_tcp.sin_addr.s_addr = INADDR_ANY;
	proxy_address_tcp.sin_family = AF_INET;
	proxy_address_tcp.sin_port = htons(9002);

	// Establishing connection
	if (connect(proxy_socket, (struct sockaddr *)&proxy_address_tcp, sizeof(proxy_address_tcp)) == -1)
	{
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	int ok = 1;
	if (send(proxy_socket, &ok, sizeof(ok), 0) == -1)
	{
		perror("Send failed");
		exit(EXIT_FAILURE);
	}

	// Assuming ventesVoitures is the array of structs to receive
	if (recv(proxy_socket, ventesVoitures, sizeof(ventesVoitures), 0) == -1)
	{
		perror("Receive failed");
		exit(EXIT_FAILURE);
	}

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
	if (client_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(9000);
	int opt = 1;
	if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("Setsockopt failed");
		exit(EXIT_FAILURE);
	}
	// Binding the socket to the port
	if (bind(client_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	// Listening for incoming connections
	if (listen(client_socket, 50) == -1)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	// Accepting incoming connection
	new_socket = accept(client_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_socket == -1)
	{
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		// Receiving the integer from client
		if (recv(new_socket, &choice, sizeof(choice), 0) == -1)
		{
			perror("Receive failed");
			exit(EXIT_FAILURE);
		}

		printf("Integer received from client: %d\n", choice);

		if (choice == 1)
		{
			Entr1Config();
			if (send(new_socket, ventesPara, sizeof(ventesPara), 0) == -1)
			{
				perror("Send failed");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			Entr2Config();
			if (send(new_socket, ventesVoitures, sizeof(ventesVoitures), 0) == -1)
			{
				perror("Send failed");
				exit(EXIT_FAILURE);
			}
		}
	}
	close(new_socket);
	close(client_socket);
	printf("Struct sent to client..\n");
	return 0;
}