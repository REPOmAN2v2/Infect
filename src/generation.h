#ifndef GENERATION_H_
#define GENERATION_H_

#include "include.h"

void parseArgs(int argc, char **argv);
void printHelp();
void generateCoord(Board board[][X], const int, Characters);
void initialise(Board board[][X]);
void defaultBoard(Board board[][X]);

#endif