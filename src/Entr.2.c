#include <stdio.h>
#include <sqlite3.h> 

typedef struct
{
  int idFacture;
  int codeClient;
  char *montant;
} VentesVoitures;

int main(int argc, char* argv[]) {
   sqlite3 *db;
   int rc;
   char *sql;
   char *ErrMsg = 0;
   rc = sqlite3_open("../sockets.db", &db);
   sql="SELECT * FROM ventesVoitures";
   rc = sqlite3_exec(db, sql, qsd, 0, &ErrMsg);
sqlite3_close(db);
}