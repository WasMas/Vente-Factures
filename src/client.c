#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef struct
{
  int idFacture;
  int codeClient;
  char montant[255];
} Ventes;
Ventes ventesVoitures[255];
int main()
{
  int clientSocket = 0;
  struct sockaddr_in serv_addr;
  int choice;
  do
  {
    printf("Choose 1 for Entr1, 2 for Entr2");
    scanf("%i", &choice);
  } while (choice != 1 && choice != 2);

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(9000);

  connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  send(clientSocket, &choice, sizeof(choice), 0);

  printf("Integer sent to server: %d\n", choice);
  recv(clientSocket, ventesVoitures, sizeof(ventesVoitures), 0);

  close(clientSocket);
  for (int i = 0; i < 3; ++i)
  {
    printf("id: %i, code client: %i, montant: %s\n", ventesVoitures[i].idFacture, ventesVoitures[i].codeClient, ventesVoitures[i].montant);
  }
  printf("\n");
  return 0;
}