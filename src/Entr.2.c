#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

typedef struct
{
  int idFacture;
  int codeClient;
  char *montant;
} VentesVoitures;

int main(int argc, char *argv[])
{
  sqlite3_stmt *statement = NULL;
  VentesVoitures ventes[255];
  sqlite3 *db;
  int rc;
  int i = 0;
  char *sql = "SELECT * FROM ventesVoitures";

  sqlite3_open("../sockets.db", &db);
  rc = sqlite3_prepare_v2(db, sql, -1, &statement, NULL);

  do
  {
    rc = sqlite3_step(statement);
    ventes[i].codeClient = sqlite3_column_int(statement, 0);
    ventes[i].idFacture = sqlite3_column_int(statement, 1);
    ventes[i].montant = sqlite3_column_text(statement, 2);
    i++;
  } while (rc != SQLITE_DONE);

  printf("%i %i %s\n", ventes[1].codeClient, ventes[1].idFacture, ventes[1].montant);
  printf("%i %i %s\n", ventes[0].codeClient, ventes[0].idFacture, ventes[0].montant);
  sqlite3_finalize(statement);
  sqlite3_close(db);

  return 0;
}