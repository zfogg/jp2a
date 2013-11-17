#include <curses.h>
#include <stdlib.h>

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
}

void drawLine(char* s) {
    if (s[0] == 9) // ch == '\t'
        wmove(win, 0, 0);
    else {
        wprintw(win, s);
        wrefresh(win);
    }
}

void killCurses() {
    endwin();
}

