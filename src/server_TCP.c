#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int num;

  // Creating socket file descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(9000);

  // Binding the socket to the port
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  // Listening for incoming connections
  listen(server_fd, 3);

  // Accepting incoming connection
  new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

  // Receiving the integer from client
  recv(new_socket, &num, sizeof(num), 0);

  printf("Integer received from client: %d\n", num);

  close(new_socket);
  close(server_fd);

  return 0;
}
