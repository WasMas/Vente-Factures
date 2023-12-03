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

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    struct MyStruct myData[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        myData[i].id = i + 1;
        myData[i].value = 3.14 * (i + 1);
        sprintf(myData[i].message, "Hello from client %d!", i + 1);
    }

    printf("Sending array of structs to server...\n");

    sendto(sockfd, (const struct MyStruct *)&myData, sizeof(myData), MSG_CONFIRM,
           (const struct sockaddr *)&serverAddr, sizeof(serverAddr));

    printf("Data sent.\n");

    char buffer[MAX_SIZE];
    int n, len;
    len = sizeof(serverAddr);
    n = recvfrom(sockfd, (char *)buffer, MAX_SIZE, MSG_WAITALL,
                 (struct sockaddr *)&serverAddr, &len);
    buffer[n] = '\0';
    printf("Received acknowledgment: %s\n", buffer);

    close(sockfd);
    return 0;
}
