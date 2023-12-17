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
	int montant;
} Ventes;

Ventes ventes[256];

int ok = 1;

void Entr1Config()
{
	struct sockaddr_in Entr1P_address;
	int Entr1P_socket, err;
	// Input integer to send to the server

	Entr1P_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (Entr1P_socket < 0)
	{
		perror("Proxy's socket creation for Entr.1 failed");
		exit(EXIT_FAILURE);
	}

	Entr1P_address.sin_family = AF_INET;
	Entr1P_address.sin_port = htons(9001);
	Entr1P_address.sin_addr.s_addr = INADDR_ANY;

	err = sendto(Entr1P_socket, &ok, sizeof(ok), 0, (const struct sockaddr *)&Entr1P_address, sizeof(Entr1P_address));
	if (err < 0)
	{
		perror("Send to server failed");
		perror("Failed to send OK to Entr1");
		exit(EXIT_FAILURE);
	}

	printf("OK sent to Entr.1: %i\n", ok);
	err = recvfrom(Entr1P_socket, &ventes, sizeof(ventes), 0, NULL, NULL);
	if (err < 0)
	{
		perror("failed to Receive array of structs from Entr.1");
		exit(EXIT_FAILURE);
	}

	printf("Received array of structs from Entr.1 successfully\n");

	close(Entr1P_socket);
}

void Entr2Config()
{
	struct sockaddr_in Entr2P_address;
	int Entr2P_socket, err;

	Entr2P_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Entr2P_socket == -1)
	{
		perror("Proxy's socket creation for Entr.2 failed");
		exit(EXIT_FAILURE);
	}

	Entr2P_address.sin_addr.s_addr = INADDR_ANY;
	Entr2P_address.sin_family = AF_INET;
	Entr2P_address.sin_port = htons(9002);

	// Establishing connection
	err = connect(Entr2P_socket, (struct sockaddr *)&Entr2P_address, sizeof(Entr2P_address));
	if (err == -1)
	{
		perror("Connection with Entr.2 failed");
		exit(EXIT_FAILURE);
	}

	send(Entr2P_socket, &ok, sizeof(ok), 0);
	if (err == -1)
	{
		perror("Failed to send OK to Entr2");
		exit(EXIT_FAILURE);
	}
	printf("OK sent to Entr.2: %i\n", ok);
	err = recv(Entr2P_socket, ventes, sizeof(ventes), 0);
	if (err == -1)
	{
		perror("failed to Receive array of structs from Entr.2");
		exit(EXIT_FAILURE);
	}
	printf("Received array of structs from Entr.2 successfully\n");
	close(Entr2P_socket);
}

int main()
{
	int clientP_socket, accept_socket, choice, err, opt = 1;
	struct sockaddr_in clientP_address;
	int addrlen = sizeof(clientP_address);

	// Creating socket file descriptor
	clientP_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientP_socket == -1)
	{
		perror("Proxy's socket creation failed");
		exit(EXIT_FAILURE);
	}

	clientP_address.sin_family = AF_INET;
	clientP_address.sin_addr.s_addr = INADDR_ANY;
	clientP_address.sin_port = htons(9000);
	err = setsockopt(clientP_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (err == -1)
	{
		perror("Setsockopt failed");
		exit(EXIT_FAILURE);
	}
	// Binding the socket to the port
	err = bind(clientP_socket, (struct sockaddr *)&clientP_address, sizeof(clientP_address));
	if (err == -1)
	{
		perror("Bind with client failed");
		exit(EXIT_FAILURE);
	}
	printf("Proxy running...\n\n");
	// Listening for incoming connections
	err = listen(clientP_socket, 50);
	if (err == -1)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	// Accepting incoming connection
	accept_socket = accept(clientP_socket, (struct sockaddr *)&clientP_address, (socklen_t *)&addrlen);
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
	close(clientP_socket);
	return 0;
}