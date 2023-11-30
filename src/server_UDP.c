#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    // Create client socket (UDP)
    int server_socket;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure l'adresse et le port sur lequel le socket écoutera
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9001);       // Choose not used Port
    server_address.sin_addr.s_addr = INADDR_ANY; // Use our LocalMachine Address

    // Associe le socket à l'adresse et au port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // Variables pour stocker l'address du client et ses donnée
    char message_client[256];
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    // Réception de données depuis un client
    memset(message_client, 0, sizeof(message_client)); // OPTIONAL: Put all array data to 0 (Since strings in c are arrays of chars)
    recvfrom(server_socket, message_client, sizeof(message_client), 0, (struct sockaddr *)&client_address, &client_address_length);
    printf("Message du client: %s\n", message_client);

    // Sending message to client
    char response_serveur[256] = "Message reçu par le serveur";
    sendto(server_socket, response_serveur, sizeof(response_serveur), 0, (struct sockaddr *)&client_address, client_address_length);

    // Close socket
    close(server_socket);

    return 0;
}