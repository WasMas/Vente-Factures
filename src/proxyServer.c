#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef struct
{
    int idFacture;
    int codeClient;
    char montant[255];
} VentesVoitures;

int main()
{
    struct sockaddr_in proxy_address_tcp;
    int proxy_socket, new_socket;

    VentesVoitures ventes[255];

    proxy_socket = socket(AF_INET, SOCK_STREAM, 0);
    proxy_address_tcp.sin_addr.s_addr = INADDR_ANY;
    proxy_address_tcp.sin_family = AF_INET;
    proxy_address_tcp.sin_port = htons(9002);
    int addrlen = sizeof(proxy_address_tcp);

    // Attaching socket to port
    bind(proxy_socket, (struct sockaddr *)&proxy_address_tcp, sizeof(proxy_address_tcp));
    while (1)
    {
        // Listening To connection
        listen(proxy_socket, 5);

        new_socket = accept(proxy_socket, (struct sockaddr *)&proxy_address_tcp, (socklen_t *)&addrlen);
        // Receiving the array of structs
        recv(new_socket, ventes, sizeof(ventes), 0);
        for (int i = 0; i < 3; ++i)
        {
            printf("id %i facture: %i, montant: %s,", ventes[i].idFacture, ventes[i].idFacture, ventes[i].montant);
        }
        printf("\n");
    }
    close(proxy_socket);
    close(new_socket);
    return 0;
}