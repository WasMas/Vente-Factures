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
  int proxy_socket_UDP, len;
  struct sockaddr_in serverAddr_UDP, clientAddr;
  VentesPara ventes[256];

  // Creating socket
  proxy_socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
  serverAddr_UDP.sin_family = AF_INET;
  serverAddr_UDP.sin_addr.s_addr = INADDR_ANY;
  serverAddr_UDP.sin_port = htons(9001);

  // Binding the socket
  bind(proxy_socket_UDP, (const struct sockaddr *)&serverAddr_UDP, sizeof(serverAddr_UDP));
  printf("Server listening on port %d...\n", 9001);

  len = sizeof(clientAddr);
  while (1)
  {
    recvfrom(proxy_socket_UDP, ventes, sizeof(ventes),
             MSG_WAITALL, (struct sockaddr *)&clientAddr, &len);


    printf("\n");
    // Processing received data (Here you can perform any operations with the received structures)
    for (int i = 0; i < 3; i++)
    {
      printf("ID: %i, Value: %i, Message: %s\n",
             ventes[i].codeClient, ventes[i].idFacture, ventes[i].montant);
    }
  }
  close(proxy_socket_UDP);
  return 0;
}
