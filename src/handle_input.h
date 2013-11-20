#include <stdbool.h>

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
void setupCurses();
void drawLine(char* s);
void *getdisplayinfo(void *arg);
void killCurses();