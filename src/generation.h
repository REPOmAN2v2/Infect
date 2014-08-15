#ifndef GENERATION_H_
#define GENERATION_H_

#include "include.h"

Board ** parseArgs(int, char **);
void printHelp();
Board ** getMap(const char*);
Board ** initBoard();
void fillBoard(Board **, int, const char*);
Board **initDefault();
void defaultBoard(Board **);
void generateCoord(Board **, const int, Characters);

#endif