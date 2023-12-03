#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct
{
  int idFacture;
  int codeClient;
  char montant[256];
} VentesPara;

int main()
{
  int proxy_socket, len;
  struct sockaddr_in serverAddr, clientAddr;
  VentesPara ventes[256];

  // Creating socket
  proxy_socket = socket(AF_INET, SOCK_DGRAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(9001);

  // Binding the socket
  bind(proxy_socket, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
  printf("Server listening on port %d...\n", 9001);

  len = sizeof(clientAddr);
  recvfrom(proxy_socket, ventes, sizeof(ventes),
           MSG_WAITALL, (struct sockaddr *)&clientAddr, &len);

  printf("Received array of structs from client:\n");

  // Processing received data (Here you can perform any operations with the received structures)
  for (int i = 0; i < 3; i++)
  {
    printf("ID: %i, Value: %i, Message: %s\n",
           ventes[i].codeClient, ventes[i].idFacture, ventes[i].montant);
  }

  close(proxy_socket);
  return 0;
}
