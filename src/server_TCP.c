#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main()
{
  // Create server socket (TCP)
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Configure l'adresse IP et le port sur lequel le socket écoutera
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);       // Choose not used Port
  server_address.sin_addr.s_addr = INADDR_ANY; // Use our LocalMachine Address

  // Associe le socket à l'adresse et au port
  bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

  // Listen For connections
  listen(server_socket, 5);

  // Accept client socket connection
  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);

  //Receive Client Message
  char client_response[254];
  recv(client_socket, &client_response, sizeof(client_response), 0);

  // Print Client Message
  printf("Message du client: %s\n", client_response);

  // Sending message to client
  char reponse_serveur[255] = "Message reçu par le serveur";
  send(client_socket, reponse_serveur, sizeof(reponse_serveur), 0);

  // Close socket
  close(server_socket);

  return 0;
}