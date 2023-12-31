#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct
{
  int idFacture;
  int codeClient;
  int montant;
} Ventes;

Ventes ventesVoitures[256];

int main()
{
  int clientSocket, choice, err, somme;
  struct sockaddr_in client_address;

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  client_address.sin_family = AF_INET;
  client_address.sin_addr.s_addr = INADDR_ANY;
  client_address.sin_port = htons(9000);
  err = connect(clientSocket, (struct sockaddr *)&client_address, sizeof(client_address));
  if (err == -1)
  {
    perror("Connection with proxy failed");
    exit(EXIT_FAILURE);
  }
  while (1)
  {
    do
    {
      printf("Choose 1 for Entr1, 2 for Entr2, 3 for Total: ");
      scanf("%i", &choice);
    } while (choice != 1 && choice != 2 && choice != 3);
    err = send(clientSocket, &choice, sizeof(choice), 0);
    if (err == -1)
    {
      perror("Send failed");
      exit(EXIT_FAILURE);
    }

    printf("Choice sent to Proxy: %i\n", choice);

    if (choice == 3)
    {
      somme = 0;
      err = recv(clientSocket, &somme, sizeof(somme), 0);
      if (err == -1)
      {
        perror("Receive failed");
        exit(EXIT_FAILURE);
      }
      printf("Total Received Successfully: %.3f TND\n", (double)somme);
    }
    else
    {
      err = recv(clientSocket, ventesVoitures, sizeof(ventesVoitures), 0);
      // Assuming ventesVoitures is the array of structs to receive
      if (err == -1)
      {
        perror("Receive failed");
        exit(EXIT_FAILURE);
      }
      printf("Struct Received Successfully:\n");

      for (int i = 0; i < 3; ++i)
      {
        printf("id: %i, code client: %i, montant: %.3f TND\n", ventesVoitures[i].idFacture, ventesVoitures[i].codeClient, (double)ventesVoitures[i].montant);
      }
    }
  }
  close(clientSocket);

  return 0;
}