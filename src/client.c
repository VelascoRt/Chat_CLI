
#include <stdio.h>
#include <stdlib.h> // string pointed to, str to an integer.
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // systemcalls makes socket.h and netinet to work
#include <sys/socket.h> // sockets sockaddr
#include <netinet/in.h> // constants and structures for internet addresses.
#include <netdb.h>

// Error Message
void error (const char *msg) { 
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, port_number, n;
  struct sockaddr_in server_address;
  struct hostent *server;

  char buffer[256];
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(1);
  }
  port_number = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  server = gethostbyname(argv[1]);

  if (server == NULL) fprintf(stderr, "ERROR, no host\n");

  bzero((char *) &server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  bcopy((char *) server->h_addr, (char *) &server_address.sin_addr.s_addr, server->h_length);
  server_address.sin_port = htons(port_number);
  if (connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) error("ERROR conection failed");
  char name[30];
  printf("Welcome to the chat!\nUse LEAVE to leave the server\n"); 
  printf("Type Below: \n");

  while (1) {
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) error("ERROR on writting");
    bzero(buffer, 255);
    n = read(sockfd,buffer,255);
    if (n < 0) error("ERROR on reading");
    printf("Server : %s\n", buffer);
    int i = strncmp("LEAVE", buffer, 5); // String to leave server
    if (i == 0) break;
  } 
  close(sockfd);
  return 0;
}

