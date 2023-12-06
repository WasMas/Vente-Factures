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
  char montant[256];
} VentesVoitures;

int main()
{
  // ? Gettin Data From DataBase
  // Opening DataBase
  sqlite3 *db;
  sqlite3_open("../sockets.db", &db);

  sqlite3_stmt *statement, *statementRow = NULL;
  VentesVoitures ventes[256];

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
    strcpy(ventes[i].montant, sqlite3_column_text(statement, 2));
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  // ? Socket Stuff
  // Sending Struct To Proxy server using TCP
  int Entr2_socket, accept_socket;
  struct sockaddr_in address;

  // Creating socket
  Entr2_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (Entr2_socket == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Configuring socket
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(9002);
  int addrlen = sizeof(address);
  int opt = 1;
  if (setsockopt(Entr2_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
  {
    perror("Setsockopt failed");
    exit(EXIT_FAILURE);
  }
  if (bind(Entr2_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
  {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(Entr2_socket, 5) == -1)
  {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  accept_socket = accept(Entr2_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  if (accept_socket == -1)
  {
    perror("Accept failed");
    exit(EXIT_FAILURE);
  }

  int ok;

  if (recv(accept_socket, &ok, sizeof(ok), 0) == -1)
  {
    perror("Receive failed");
    exit(EXIT_FAILURE);
  }
  printf("Entr.2: RECEIVED THE OK %i\n", ok);

  // Sending the struct
  if (send(accept_socket, ventes, sizeof(ventes), 0) == -1)
  {
    perror("Send failed");
    exit(EXIT_FAILURE);
  }
  printf("Array of structs sent\n");

  close(Entr2_socket);
  close(accept_socket);
  return 0;
}