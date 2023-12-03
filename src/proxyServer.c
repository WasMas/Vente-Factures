#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
typedef struct
{
    int idFacture;
    int codeClient;
    char montant[255];
} VentesVoitures;

typedef struct
{
    int idFacture;
    int codeClient;
    char montant[256];
} VentesPara;

void *Entr1Config(void *arg)
{
    int proxy_socket_UDP, len;
    struct sockaddr_in serverAddr_UDP, clientAddr;
    VentesPara ventes[256];

    // Creating socket
    proxy_socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr_UDP.sin_family = AF_INET;
    serverAddr_UDP.sin_addr.s_addr = INADDR_ANY;
    serverAddr_UDP.sin_port = htons(9001);

    // Binding the socket
    bind(proxy_socket_UDP, (const struct sockaddr *)&serverAddr_UDP, sizeof(serverAddr_UDP));

    len = sizeof(clientAddr);
    while (1)
    {
        recvfrom(proxy_socket_UDP, ventes, sizeof(ventes),
                 MSG_WAITALL, (struct sockaddr *)&clientAddr, &len);

        printf("Entr1 Sent Stuff:\n");
        // Processing received data (Here you can perform any operations with the received structures)
        for (int i = 0; i < 3; i++)
        {
            printf("ID: %i, Value: %i, Message: %s\n",
                   ventes[i].codeClient, ventes[i].idFacture, ventes[i].montant);
        }
    }
    close(proxy_socket_UDP);
    return NULL;
}

void *Entr2Config(void *arg)
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
        printf("Entr2 Sent Stuff:\n");
        for (int i = 0; i < 3; ++i)
        {
            printf("id %i facture: %i, montant: %s,", ventes[i].idFacture, ventes[i].idFacture, ventes[i].montant);
        }
        printf("\n");
    }
    close(proxy_socket);
    close(new_socket);
    return NULL;
}

int main()
{
    pthread_t entr1Thread, entr2Thread;

    // Creating threads for handling ENTR1 and ENTR2 connections
    pthread_create(&entr1Thread, NULL, Entr1Config, NULL);
    pthread_create(&entr2Thread, NULL, Entr2Config, NULL);

    // Waiting for threads to finish
    pthread_join(entr1Thread, NULL);
    pthread_join(entr2Thread, NULL);

    return 0;
}