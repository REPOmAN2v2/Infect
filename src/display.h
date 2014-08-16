#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "include.h"

void initNcurses();
int displayBoard(Board **);
void printError(const char *);
void printHelp();
void printVersion();

#endif