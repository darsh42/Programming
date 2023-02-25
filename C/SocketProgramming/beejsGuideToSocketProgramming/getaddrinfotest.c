#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv) {
  int status;
  char ipstr[INET6_ADDRSTRLEN];
  struct addrinfo hints, *serverinfo, *pointer;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], NULL, &hints, &serverinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(EXIT_FAILURE);
  }
  // Do all things with server info 

  printf("IP addresses for %s:\n\n", argv[1]);

  for (pointer = serverinfo; pointer != NULL; pointer = pointer->ai_next){
    void *addr;
    char *ipver;

    if (pointer->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *) pointer->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) pointer->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
  
    inet_ntop(pointer->ai_family, addr, ipstr, sizeof(ipstr));
    printf("  %s: %s\n", ipver, ipstr);

  }

  // free linked list after use
  freeaddrinfo(serverinfo);

  return 0;
}
