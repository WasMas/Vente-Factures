#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXCHAR 1000

typedef struct
{
  int idFacture;
  int codeClient;
  char montant[256];
} VentesPara;

int main()
{

  FILE *csvFile = fopen("/home/wasmas/Vente-Factures/Other/VentesParapharmaceutiques.csv", "r");
  VentesPara ventes[248];

  char *buf = malloc(256);
  char *tmp;
  fgets(buf, 256, csvFile) != NULL;
  int i = 0;
  while (fgets(buf, 256, csvFile) != NULL)
  {
    if ((strlen(buf) > 0) && (buf[strlen(buf) - 1] == '\n'))
      buf[strlen(buf) - 1] = '\0';

    tmp = strtok(buf, ",");
    ventes[i].codeClient = atoi(tmp);

    tmp = strtok(NULL, ",");
    ventes[i].idFacture = atoi(tmp);

    tmp = strtok(NULL, ",");
    strcpy(ventes[i].montant, tmp);

    i++;
  }
  fclose(csvFile);

  // ? Socket Stuff

  struct sockaddr_in serverAddr, clientAddr;
  int sockfd, len, n;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(9001);

  if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
  {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Server running...\n");

  int receivedInt;

  len = sizeof(clientAddr);
  n = recvfrom(sockfd, &receivedInt, sizeof(receivedInt), 0, (struct sockaddr *)&clientAddr, &len);
  if (n < 0)
  {
    perror("Receive from client failed");
    exit(EXIT_FAILURE);
  }

  printf("Received integer from client: %d\n", receivedInt);

  n = sendto(sockfd, &ventes, sizeof(ventes), 0, (const struct sockaddr *)&clientAddr, len);
  if (n < 0)
  {
    perror("Send to client failed");
    exit(EXIT_FAILURE);
  }

  printf("Array of structs sent to client\n");

  close(sockfd);
  return 0;
}