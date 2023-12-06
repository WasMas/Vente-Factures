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

  struct sockaddr_in Entr1_addr, proxy_address;
  int Entr1_socket, err;

  Entr1_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (Entr1_socket < 0)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  Entr1_addr.sin_family = AF_INET;
  Entr1_addr.sin_addr.s_addr = INADDR_ANY;
  Entr1_addr.sin_port = htons(9001);
  err = bind(Entr1_socket, (const struct sockaddr *)&Entr1_addr, sizeof(Entr1_addr));
  if (err < 0)
  {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Entr.1 running...\n\n");

  int OK;

  int Entr1_addrlen = sizeof(proxy_address);
  while (1)
  {
    err = recvfrom(Entr1_socket, &OK, sizeof(OK), 0, (struct sockaddr *)&proxy_address, &Entr1_addrlen);
    if (err < 0)
    {
      perror("Failed to receive OK from Proxy");
      exit(EXIT_FAILURE);
    }

    printf("Received OK from proxy: %i\n", OK);
    err = sendto(Entr1_socket, &ventes, sizeof(ventes), 0, (const struct sockaddr *)&proxy_address, Entr1_addrlen);
    if (err < 0)
    {
      perror("Failed to send array of structs to Proxy");
      exit(EXIT_FAILURE);
    }

    printf("Array of structs sent to proxy\n");
  }
  close(Entr1_socket);
  return 0;
}