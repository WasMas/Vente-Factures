#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
  // Create client socket (UDP)
  int client_socket;
  client_socket = socket(AF_INET, SOCK_DGRAM, 0);

  // Configure l'adresse et le port sur lequel le socket écoutera
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9001); // Choose Port not used
  server_address.sin_addr.s_addr = INADDR_ANY; // Use our LocalMachine Address

  // Envoie d'un message au serveur
  char message_serveur[254] = "Hello, serveur UDP!";
  sendto(client_socket, message_serveur, strlen(message_serveur), 0, (struct sockaddr *)&server_address, sizeof(server_address));

  // Variables pour stocker les donnée serveur
  char response_serveur[254];
  socklen_t server_address_length = sizeof(server_address);

  // Réception de données depuis un client
  memset(response_serveur, 0, sizeof(response_serveur)); // OPTIONAL: Put all array data to 0 (Since strings in c are arrays of chars)
  recvfrom(client_socket, response_serveur, sizeof(response_serveur), 0, (struct sockaddr *)&server_address, &server_address_length);
  printf("Serveur: %s\n", response_serveur);

  //Close Socket
  close(client_socket);

  return 0;
}