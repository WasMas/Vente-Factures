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
Ventes ventes[256];
int ok = 1;
void Entr1Config()
{
	struct sockaddr_in serverAddr;
	int sockfd, err;
	// Input integer to send to the server

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("Proxy's socket creation for Entr.2 failed");
		exit(EXIT_FAILURE);
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9001);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	err = sendto(sockfd, &ok, sizeof(ok), 0, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (err < 0)
	{
		perror("Send to server failed");
		perror("Failed to send OK to Entr1");
		exit(EXIT_FAILURE);
	}

	printf("OK sent to Entr.1: %i\n", ok);
	err = recvfrom(sockfd, &ventes, sizeof(ventes), 0, NULL, NULL);
	if (err < 0)
	{
		perror("failed to Receive array of structs from Entr.1");
		exit(EXIT_FAILURE);
	}

	printf("Received array of structs from Entr.1 successfully\n");

	close(sockfd);
}

void Entr2Config()
{
	struct sockaddr_in proxy_address_tcp;
	int proxy_socket, err;

	proxy_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (proxy_socket == -1)
	{
		perror("Proxy's socket creation for Entr.2 failed");
		exit(EXIT_FAILURE);
	}

	proxy_address_tcp.sin_addr.s_addr = INADDR_ANY;
	proxy_address_tcp.sin_family = AF_INET;
	proxy_address_tcp.sin_port = htons(9002);

	// Establishing connection
	err = connect(proxy_socket, (struct sockaddr *)&proxy_address_tcp, sizeof(proxy_address_tcp));
	if (err == -1)
	{
		perror("Connection with Entr.2 failed");
		exit(EXIT_FAILURE);
	}

	send(proxy_socket, &ok, sizeof(ok), 0);
	if (err == -1)
	{
		perror("Failed to send OK to Entr2");
		exit(EXIT_FAILURE);
	}
	printf("OK sent to Entr.2: %i\n", ok);
	err = recv(proxy_socket, ventes, sizeof(ventes), 0);
	if (err == -1)
	{
		perror("failed to Receive array of structs from Entr.2");
		exit(EXIT_FAILURE);
	}
	printf("Received array of structs from Entr.2 successfully\n");
	close(proxy_socket);
}

int main()
{
	int client_socket, accept_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int choice, err;

	// Creating socket file descriptor
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
	{
		perror("Proxy's socket creation failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(9000);
	int opt = 1;
	err = setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (err == -1)
	{
		perror("Setsockopt failed");
		exit(EXIT_FAILURE);
	}
	// Binding the socket to the port
	err = bind(client_socket, (struct sockaddr *)&address, sizeof(address));
	if (err == -1)
	{
		perror("Bind with client failed");
		exit(EXIT_FAILURE);
	}
	printf("Proxy running...\n\n");
	// Listening for incoming connections
	err = listen(client_socket, 50);
	if (err == -1)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	// Accepting incoming connection
	accept_socket = accept(client_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (accept_socket == -1)
	{
		perror("Client socket acceptance failed");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		// Receiving the integer from client
		err = recv(accept_socket, &choice, sizeof(choice), 0);
		if (err == -1)
		{
			perror("Failed to receive choice from client");
			exit(EXIT_FAILURE);
		}

		printf("Choice received from client: %d\n", choice);

		if (choice == 1)
		{
			Entr1Config();
			err = send(accept_socket, ventes, sizeof(ventes), 0);
			if (err == -1)
			{
				perror("Proxy Failed to send Entr.1 array of structs to client");
				exit(EXIT_FAILURE);
			}
			printf("Entr.1 Struct sent to client..\n");
		}
		else
		{
			Entr2Config();
			err = send(accept_socket, ventes, sizeof(ventes), 0);
			if (err == -1)
			{
				perror("Proxy failed to send Entr.2 array of structs to client");
				exit(EXIT_FAILURE);
			}
			printf("Entr.2 Struct sent to client..\n");
		}
	}
	close(accept_socket);
	close(client_socket);
	return 0;
}