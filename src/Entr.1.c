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

  int Entr1_socket;
  struct sockaddr_in address;

  // Creating socket
  Entr1_socket = socket(AF_INET, SOCK_DGRAM, 0);
  address.sin_family = AF_INET;
  address.sin_port = htons(9001);
  address.sin_addr.s_addr = INADDR_ANY;
  while (1)
  {
    // Sending Socket
    sendto(Entr1_socket, ventes, sizeof(ventes), MSG_CONFIRM,
           (const struct sockaddr *)&address, sizeof(address));
    printf("Data sent.\n");
  }
  close(Entr1_socket);
  return 0;
}