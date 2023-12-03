#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_SIZE 1024
#define ARRAY_SIZE 3 // Change the array size as needed

struct MyStruct {
    int id;
    float value;
    char message[256];
};

typedef struct
{
  int idFacture;
  int codeClient;
  char montant[255];
} VentesPara;

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    struct MyStruct receivedStruct[ARRAY_SIZE];
    VentesPara ventes[255];


    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    int len, n;
    len = sizeof(clientAddr);
    while (1) {
        n = recvfrom(sockfd, (struct VentesPara *)&ventes, sizeof(receivedStruct), 
                     MSG_WAITALL, (struct sockaddr *)&clientAddr, &len);

        printf("Received array of structs from client:\n");

        // Processing received data (Here you can perform any operations with the received structures)
        for (int i = 0; i < 3; i++) {
            printf("ID: %i, Value: %i, Message: %s\n",
                   ventes[i].codeClient, ventes[i].idFacture, ventes[i].montant);
        }

        // Sending acknowledgment
        sendto(sockfd, "ACK", strlen("ACK"), MSG_CONFIRM,
               (const struct sockaddr *)&clientAddr, len);
    }

    close(sockfd);
    return 0;
}
