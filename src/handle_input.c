#include <curses.h>
#include <stdlib.h>

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


WINDOW * win;

typedef struct clientframe {
    bool isConnected;
    int new_fd;
    int disp_fd;
} Clientframe;
Clientframe cf;


void *startlisten(void*);
void *getdisplayinfo(void*);
void reallystartlisten(Clientframe*);
void sendframe(int, char*, int);


void setupCurses() {
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    win = newwin(maxY, maxX, 0, 0);
    reallystartlisten(&cf);

    pthread_t tid2;
    pthread_create(&tid2, NULL, getdisplayinfo, (void *)&cf);
}

void drawLine(char* s) {
    if (s[0] == 9) // ch == '\t'
        wmove(win, 0, 0);
    else {
        wprintw(win, s);
        wrefresh(win);
    }
}

void *getdisplayinfo(void *arg) {
  Clientframe *cf = (void*)arg;
  char* frame = malloc(10000);
  while (recv(cf->disp_fd, frame, 10000, 0) != -1) {
    drawLine(frame);
  }
}

void killCurses() {
    endwin();
}

void *startlisten(void *arg) {
    Clientframe *cf = (Clientframe*)arg;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM; // tcp
    hints.ai_flags = AI_PASSIVE; // fill in ip

    int status;
    char* port = "9337";
    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      return;
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
    int disp_fd;
    disp_fd = accept(sock, (struct sockaddr *)&their_addr, &addr_size);

    cf->isConnected = true;
    cf->new_fd      = new_fd;
    cf->disp_fd     = disp_fd;
}

// current: client connects to server, server sends frames to client which then sends frames
// to partner.
// We receive frames from the partner and send it to the server which displays in terminal.
// need thread to listen for frames and display them

void reallystartlisten(Clientframe *cf) {
    pthread_t tid;
    pthread_create(&tid, NULL, startlisten, (void *) cf);

}

void sendframe(int fd, char* s, int len) {
    send(fd, s, len, 0);
}
