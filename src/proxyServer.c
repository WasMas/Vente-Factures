#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct
{
    int idFacture;
    int codeClient;
    char montant[255];
} VentesVoitures;

int main()
{
    struct sockaddr_in proxy_address_tcp;
    int proxy_socket = 0;
    VentesVoitures ventes[255];

proxy_socket = socket(AF_INET, SOCK_STREAM, 0);

proxy_address_tcp.sin_family = AF_INET;
proxy_address_tcp.sin_port = htons(9002);

connect(proxy_socket, (struct sockaddr *)&proxy_address_tcp, sizeof(proxy_address_tcp));

// Receiving the array of structs
recv(proxy_socket, ventes, sizeof(ventes), 0);

printf("Received array of structs From Entr.2:\n");
for (int i = 0; i < 3; ++i)
{
    printf("id %i facture: %i, montant: %s,", ventes[i].idFacture, ventes[i].idFacture, ventes[i].montant);
}
return 0;
}
