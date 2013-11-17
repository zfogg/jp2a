#include <pthread.h>
#include <errno.h> // debugging
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


int main(void) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM; // tcp
    hints.ai_flags = AI_PASSIVE; // fill in ip
    
    struct addrinfo *res;
    
    int status;
    char* port = "9337";
    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      return 1;
    }
    
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
      printf("%s\n", "error with socket :x");
    }
    
    int bnd = bind(sock, res->ai_addr, res->ai_addrlen);
    if (bnd == -1) {
      printf("%s\n", "error with bind!");
    }
    
    int listener = listen(sock, 20); // listening!
    if (listener == -1) {
      printf("%s\n", "error with listener");
    }
    
    socklen_t addr_size;
    struct sockaddr_storage their_addr;
    addr_size = sizeof their_addr;
    int new_fd;
    new_fd = accept(sock, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd) {
      char* message = malloc(10000);
      strcpy(message, "dicks\0");
      send(new_fd, message, 8, 0);
    }
    return 0;
}
