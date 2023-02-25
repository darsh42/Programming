#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFSIZE 2000
#define IPBUFF 20


int main(int argc, char **argv) {
  
  // getaddrinfo params
  struct addrinfo hints;
  struct addrinfo *result, *resultPointer;
  char *node = argv[1];
  char *service = "http";

  int sfd, status;
  struct sockaddr_in *server;

  char ip[IPBUFF];
  char *message;
  char serv_reply[BUFFSIZE];
  
  // Define hints for getaddrinfo
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_flags = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  if ((status = getaddrinfo(node, service, &hints, &result)) != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(EXIT_FAILURE);
  }

  for (resultPointer = result; resultPointer != NULL; resultPointer = resultPointer->ai_next) {

    printf("%s resolved to: %s\n", node, inet_ntoa(((struct sockaddr_in *)resultPointer->ai_addr)->sin_addr));

    if ((sfd = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol) < 0))
        continue;

    break;
  }

  if (connect(sfd, resultPointer->ai_addr, sizeof(*(resultPointer->ai_addr))) < 0) {
    fprintf(stderr, "connect: %s\n", strerror(errno));
  }

  puts("connected");

  message = "GET / HTTP/1.1\r\n\r\n";
  if (send(sfd, message, strlen(message), 0) < 0) {
    printf("send: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  puts("request sent");


  if (recv(sfd, serv_reply, 2000, 0) < 0) {
    printf("recv: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  puts("reply recived");

  read(sfd, serv_reply, 2000);
  printf("reply: %c\n", serv_reply[0]);
  puts(serv_reply);
  close(sfd);
  //strcpy(ip, inet_ntoa(((struct sockaddr_in *)resultPointer->ai_addr)->sin_addr));

  

  return 0;
}
