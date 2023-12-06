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
} Ventes;
Ventes ventesVoitures[256];

int main()
{
  int clientSocket;
  struct sockaddr_in serv_addr;
  int choice;

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(9000);

  connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  do
  {
    printf("Choose 1 for Entr1, 2 for Entr2");
    scanf("%i", &choice);
  } while (choice != 1 && choice != 2);
  send(clientSocket, &choice, sizeof(choice), 0);

  printf("Integer sent to server: %d\n", choice);
  recv(clientSocket, ventesVoitures, sizeof(ventesVoitures), 0);
  for (int i = 0; i < 3; ++i)
  {
    printf("id: %i, code client: %i, montant: %s\n", ventesVoitures[i].idFacture, ventesVoitures[i].codeClient, ventesVoitures[i].montant);
  }
  printf("\n");

  close(clientSocket);

  return 0;
}