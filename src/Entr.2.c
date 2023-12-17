#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

typedef struct
{
  int idFacture;
  int codeClient;
  int montant;
} VentesVoitures;

int main()
{
  // ? Gettin Data From DataBase
  // Opening DataBase
  sqlite3 *db;
  sqlite3_open("../Other/sockets.db", &db);

  sqlite3_stmt *statement, *statementRow = NULL;
  VentesVoitures ventes[256];
  memset(ventes, 0, sizeof(ventes));
  int rc, rc1, rowCount;
  char *sql = "SELECT * FROM ventesVoitures";
  char *sqlRowCount = "SELECT count(*) from ventesVoitures";

  rc = sqlite3_prepare_v2(db, sql, -1, &statement, NULL);
  rc1 = sqlite3_prepare_v2(db, sqlRowCount, -1, &statementRow, NULL);

  rc1 = sqlite3_step(statementRow);
  rowCount = sqlite3_column_int(statementRow, 0);

  for (int i = 0; i < rowCount; i++)
  {
    rc = sqlite3_step(statement);
    ventes[i].idFacture = sqlite3_column_int(statement, 0);
    ventes[i].codeClient = sqlite3_column_int(statement, 1);
    ventes[i].montant = sqlite3_column_int(statement, 2);
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  // ? Socket Stuff
  // Sending Struct To Proxy server using TCP
  int Entr2_socket, accept_socket, err, opt = 1, ok;
  struct sockaddr_in Entr2_address;

  // Creating socket
  Entr2_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (Entr2_socket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Configuring socket
  Entr2_address.sin_family = AF_INET;
  Entr2_address.sin_addr.s_addr = INADDR_ANY;
  Entr2_address.sin_port = htons(9002);
  int Entr2_addrlen = sizeof(Entr2_address);

  err = setsockopt(Entr2_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (err == -1)
  {
    perror("Setsockopt failed");
    exit(EXIT_FAILURE);
  }
  err = bind(Entr2_socket, (struct sockaddr *)&Entr2_address, sizeof(Entr2_address));
  if (err == -1)
  {
    perror("Bind with proxy failed");
    exit(EXIT_FAILURE);
  }
  err = listen(Entr2_socket, 5);
  if (err == -1)
  {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }
  printf("Entr.2 running...\n\n");

  while (1)
  {
    accept_socket = accept(Entr2_socket, (struct sockaddr *)&Entr2_address, (socklen_t *)&Entr2_addrlen);
    if (accept_socket == -1)
    {
      perror("Accept failed");
      exit(EXIT_FAILURE);
    }
    err = recv(accept_socket, &ok, sizeof(ok), 0);
    if (err == -1)
    {
      perror("Failed to receive OK from Proxy");
      exit(EXIT_FAILURE);
    }
    printf("Received OK from proxy: %i\n", ok);

    // Sending the struct
    err = send(accept_socket, ventes, sizeof(ventes), 0);
    if (err == -1)
    {
      perror("Failed to send array of structs to Proxy");
      exit(EXIT_FAILURE);
    }
    printf("Array of structs sent to proxy\n");
  }
  close(Entr2_socket);
  close(accept_socket);

  return 0;
}