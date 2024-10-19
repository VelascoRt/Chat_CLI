
#include <stdio.h>
#include <stdlib.h> // string pointed to, str to an integer.
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // systemcalls makes socket.h and netinet to work
#include <sys/socket.h> // sockets sockaddr
#include <netinet/in.h> // constants and structures for internet addresses.

// Error Message
void error (const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "ERROR Port number not provided. Program terminated\n");
    exit(1);
  }
  int sockfd, newsockdf, port_number, n;
  char buffer[255];

  struct sockaddr_in server_address, client_address;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0); // Socket
  if (sockfd < 0) {
    error("ERROR OPENING SOCKET.");
  }

  bzero((char *) &server_address, sizeof(server_address));
  port_number = atoi(argv[1]);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port_number);

  if (bind(sockfd, (struct sockaddr *) &server_address , sizeof(server_address)) < 0)
    error("BINDING FAILED.");

  listen(sockfd, 5); // max client connection size 
  clilen = sizeof(client_address);

  newsockdf = accept(sockfd, (struct sockaddr *) &client_address, &clilen);
  
  if(newsockdf < 0) error("ERROR on accept");


  printf("Welcome to the chat!\nUse LEAVE to leave the server\n");
  printf("Type Below: \n");
  while (1) {
    bzero(buffer, 255);
    n = read(newsockdf, buffer, 255);
    if (n < 0) error("ERROR on reading");
    printf("Client : %s\n", buffer);
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(newsockdf, buffer, strlen(buffer));
    if (n < 0) error("ERROR on Writting");
    int i = strncmp("LEAVE", buffer, 5); // String to leave server
    if (i == 0) break;
  }
  close(newsockdf);
  close(sockfd);
  return 0;
}






