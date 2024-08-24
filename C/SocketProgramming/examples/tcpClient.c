#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_ADDRESS "142.251.36.46"

int main(int argc, char *argv[]) {
  /* socket_desc --> non-negetive int upon sucess, is called a decscriptor
   *
   * socket(  
   *          *domain*      AF_INET      --> sockets which share common comms properies.
   *                                    e.g naming and protocol formats
   *
   *            *type*  SOCK_STREAM      --> type of socket to be created, disctate
   *                                    what protocol family is used, e.g TCP, UDP, ICMP 
   *
   *        *protocol*            0      --> specifies what protocol to use in family
   *                                     of protocols by integer, e.g 0 --> is an un-specified
   *                                     default protocol for given socket type 
   *        )
   *                                                                                            */
  int socket_desc;

  /* sockaddr_in, store addr of server in Internet connections. They must also be compatible with
   * the connect function defined in socket.h
   *
   * struct sockaddr_in {
   *      short             sin_family;
   *      unsigned short    sin_port;
   *      struct in_addr    sin_addr;
   *      char              sin_zero[8];
   * }
   *
   *  in_addr stores the IPV4 address, each of the bytes corresonds to a value in range 0 - 255
   *  e.g. 192.255.255.255
   *
   * struct in_addr {
   *      unsigned long     s_addr;
   * }
   *
   *                                                                                            */

  struct sockaddr_in server;
  char *message, server_reply[2000];

  // make socket 
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
    printf("Could not create socket");

  server.sin_addr.s_addr = inet_addr(IP_ADDRESS);
  server.sin_family = AF_INET;
  server.sin_port = htons(80);

  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
    puts("connection error");
    return 1;
  }

  puts("connected");

  // Send data
  message = "GET /  HTTP/1.1\r\n\r\n";
  if ( send(socket_desc, message, strlen(message), 0) < 0) {
    puts("Send failed");
    return 1;
  }
  
  puts("Data send\n");

  //Recieve a reply from the server
  if (recv(socket_desc, server_reply, 2000, 0) < 0)
    puts("Recieve failed");
  
  puts("Reply recieved\n");
  read(socket_desc, server_reply, 2000);
  puts(server_reply);
  close(socket_desc);
  return 0;
}
