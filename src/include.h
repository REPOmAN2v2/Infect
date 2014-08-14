#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

typedef enum _directions {NORTH, WEST, SOUTH, EAST} Directions;
typedef enum _deltas {NONE, N=-1, W=-1, S=1, E=1} Deltas;
typedef enum _characters {EMPTY,DEAD, INF, DOC, CIT, SOL, NUR} Characters;

typedef struct _board {
	Characters character;
	int direction;
} Board;

extern unsigned int 	countDoc, countInf, countNur, countSol, 
				countCit, countDea, elapsed;
extern const unsigned int localTimeout;
extern unsigned int X, Y;

void checkWin(Board board[][X], unsigned int);
void win(Board board[][X], int, unsigned int);

#endif