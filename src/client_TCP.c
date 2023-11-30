#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
int main()
{
  // Create client socket (TCP)
  int client_socket;
  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Configure l'adresse et le port sur lequel le socket écoutera
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);       // Choose Port not used
  server_address.sin_addr.s_addr = INADDR_ANY; // Use our LocalMachine Address

  // Connecting to server Socket
  connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

  // Envoie d'un message au serveur
  char message_serveur[254] = "Hello, serveur TCP!";
  send(client_socket, message_serveur, sizeof(message_serveur), 0);

  // Receive data from server
  char server_response[254];
  recv(client_socket, &server_response, sizeof(server_response), 0);

  // Print server Response
  printf("Serveur: %s\n", server_response);

  // Close socket
  close(client_socket);

  return 0;
}