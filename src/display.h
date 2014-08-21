#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "include.h"

void initNcurses();
int displayBoard(Board **board);
void printError(const char *error);
void printHelp();
void printVersion();
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#endif