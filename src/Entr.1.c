#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define MAXCHAR 1000

typedef struct
{
  int idFacture;
  int codeClient;
  char *montant;
} VentesPara;

int main()
{
  /* FileStream for the Library File */
  FILE *csvFile = fopen("/home/wasmas/Coding/Vente Factures/Other/VentesParapharmaceutiques.csv", "r");
  VentesPara ventes[255];
  /* allocation of the buffer for every line in the File */
  char *buf = malloc(255);
  char *tmp;
  fgets(buf, 255, csvFile) != NULL;
  int i = 0;
  while (fgets(buf, 255, csvFile) != NULL)
  {
    if ((strlen(buf) > 0) && (buf[strlen(buf) - 1] == '\n'))
      buf[strlen(buf) - 1] = '\0';

    tmp = strtok(buf, ",");
    ventes[i].codeClient = atoi(tmp);

    tmp = strtok(NULL, ",");
    ventes[i].idFacture = atoi(tmp);

    tmp = strtok(NULL, ",");
    ventes[i].montant = tmp;
    // tempBook.ID = atoi(buf);
    printf("index i= %i  ID: %i,codeClient: %i,montant: %s\n", i, ventes[i].codeClient, ventes[i].idFacture, ventes[i].montant);
    i++;
  }
  // free(buf);
  fclose(csvFile);
  return 0;
}