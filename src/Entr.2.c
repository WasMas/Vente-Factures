#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

typedef struct
{
  int idFacture;
  int codeClient;
  char montant[100];
} VentesVoitures;

int main(int argc, char *argv[])


{
  //Opening DataBase
  sqlite3 *db;
  sqlite3_open("../sockets.db", &db);


  sqlite3_stmt *statement, *statementRow = NULL;
  VentesVoitures ventes[255];

  int rc,rc1,rowCount;
  char* sql = "SELECT * FROM ventesVoitures";
  char* sqlRowCount="SELECT count(*) from ventesVoitures";
  
  rc = sqlite3_prepare_v2(db, sql, -1, &statement, NULL);
  rc1 = sqlite3_prepare_v2(db, sqlRowCount, -1, &statementRow, NULL);
  
  rc1 = sqlite3_step(statementRow);
  rowCount = sqlite3_column_int(statementRow, 0);


  for (int i=0; i<rowCount;i++){
    rc = sqlite3_step(statement);
    ventes[i].codeClient = sqlite3_column_int(statement, 0);
    ventes[i].idFacture = sqlite3_column_int(statement, 1);
    strcpy(ventes[i].montant,sqlite3_column_text(statement, 2));
  }
  
  printf("%i %i %s\n", ventes[2].codeClient, ventes[2].idFacture, ventes[0].montant);

  sqlite3_finalize(statement);
  sqlite3_close(db);

  return 0;
}