#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_ITEMS 5

struct Item {
    int id;
    float price;
    char name[50];
};

int main() {
    struct Item items[MAX_ITEMS] = {
        {1, 10.5, "Item 1"},
        {2, 20.3, "Item 2"},
        {3, 15.2, "Item 3"},
        {4, 8.7, "Item 4"},
        {5, 12.0, "Item 5"}
    };

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Sending the array of structs
    send(new_socket, items, sizeof(items), 0);
    printf("Array of structs sent\n");
    return 0;
}
