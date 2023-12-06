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
  if (clientSocket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(9000);

  if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }
  while (1)
  {
    do
    {
      printf("Choose 1 for Entr1, 2 for Entr2: ");
      scanf("%i", &choice);
    } while (choice != 1 && choice != 2);
    if (send(clientSocket, &choice, sizeof(choice), 0) == -1)
    {
      perror("Send failed");
      exit(EXIT_FAILURE);
    }

    printf("Integer sent to server: %d\n", choice);

    // Assuming ventesVoitures is the array of structs to receive
    if (recv(clientSocket, ventesVoitures, sizeof(ventesVoitures), 0) == -1)
    {
      perror("Receive failed");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; ++i)
    {
      printf("id: %i, code client: %i, montant: %s\n", ventesVoitures[i].idFacture, ventesVoitures[i].codeClient, ventesVoitures[i].montant);
    }
    printf("\n");
  }
  close(clientSocket);

  return 0;
}