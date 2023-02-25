#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFFSIZE 10000

/* Function will return a void pointer to the address casted to either
  IPv4 or IPv6 depending on the type of IP */
void *get_addr_in(struct sockaddr *addr);

int main(int argc, char **argv) {

  if (argc == 0){
    perror("No IP specified");
    exit(EXIT_FAILURE);
  }

  int sfd, s;
  char ip[INET6_ADDRSTRLEN];
  struct addrinfo hints, *res, *p;

  int buffLen;
  char buffer[BUFFSIZE];


  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((s = getaddrinfo(argv[1], "https", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  // Looping through all the resukts we get and try to connect to 
  // the first connectable one
  for (p = res; p != NULL; p = p->ai_next) {

    printf("client socket: Establishing\n");

    if ((sfd = socket(p->ai_family, SOCK_STREAM, 0)) == -1) {
      perror("client: socket\n");
      continue;
    }

    printf("client socket: Established\n");

    if (connect(sfd, p->ai_addr, p->ai_addrlen) < 0) {
      close(sfd);
      fprintf(stderr, "client: connection: %s\n", strerror(errno));
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client failed to connect\n");
    exit(EXIT_FAILURE);
  }

  inet_ntop(p->ai_family, get_addr_in((struct sockaddr *) p->ai_addr), ip, INET6_ADDRSTRLEN);
  printf("Client connection to: %s\n", ip);


  char *msg = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: fetch.c\r\n\r\n";
  if (send(sfd, msg, sizeof(msg), 0) < 0) {
    fprintf(stderr, "client send: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }


  if ((buffLen = recv(sfd, buffer, BUFFSIZE, 0))  == -1) {
    perror("recv: failed to recieve\n");
    close(sfd);
    exit(EXIT_FAILURE);
  }

  char page[BUFFSIZE];
  inet_ntop(p->ai_family, buffer, page, buffLen);
  puts(page);
  close(sfd);
  return 0;
}


void *get_addr_in(struct sockaddr *addr) {
  if ( addr->sa_family == AF_INET )
    return &(((struct sockaddr_in *) addr)->sin_addr);

  return &(((struct sockaddr_in6 *) addr)->sin6_addr);
}
