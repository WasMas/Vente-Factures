#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_ITEMS 5

struct Item {
    int id;
    float price;
    char name[50];
};

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    struct Item received_items[MAX_ITEMS];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Receiving the array of structs
    recv(sock, received_items, sizeof(received_items), 0);

    printf("Received array of structs:\n");
    for (int i = 0; i < MAX_ITEMS; ++i) {
        printf("Item %d - ID: %d, Price: %.2f, Name: %s\n", i + 1, received_items[i].id, received_items[i].price, received_items[i].name);
    }
    return 0;
}
