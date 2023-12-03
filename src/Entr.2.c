#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

typedef struct
{
  int idFacture;
  int codeClient;
  char montant[255];
} VentesVoitures;

int main()
{
  // ? Gettin Data From DataBase
  // Opening DataBase
  sqlite3 *db;
  sqlite3_open("../sockets.db", &db);

  sqlite3_stmt *statement, *statementRow = NULL;
  VentesVoitures ventes[255];

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
    ventes[i].codeClient = sqlite3_column_int(statement, 0);
    ventes[i].idFacture = sqlite3_column_int(statement, 1);
    strcpy(ventes[i].montant, sqlite3_column_text(statement, 2));
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  // ? Socket Stuff
  // Seding Struct To Proxy server using TCP
  int Entr2_socket, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  // Creating socket
  Entr2_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Configuring socket
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(9002);

  // Attaching socket to port
  bind(Entr2_socket, (struct sockaddr *)&address, sizeof(address));

  // Listening To connection
  listen(Entr2_socket, 5);

  new_socket = accept(Entr2_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);

  // Sending the struct
  send(new_socket, ventes, sizeof(ventes), 0);
  printf("Entr.2: Array of structs sent\n");
  return 0;
}