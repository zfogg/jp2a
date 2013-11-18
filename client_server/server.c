#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1) {
        printf("Waiting for a connection...\n");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        printf("Connection initiated, sending data.\n");
        
        /* repeatedly reset the buffer, fill with new line, then send to socket */
        int i = 0;
        while (i < 5) {
            memset(sendBuff, '0', sizeof(sendBuff));  // reset the buffer with 0s
            snprintf(sendBuff, sizeof(sendBuff), "The number %d.\n", i);
            write(connfd, sendBuff, strlen(sendBuff));
            i += 1;
        }

        printf("Closing connection.\n---------------------\n");
        close(connfd);
    }
}
