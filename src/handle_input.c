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
#include "handle_input.h"

WINDOW * win;

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
    return 0;
}

void killCurses() {
    endwin();
}