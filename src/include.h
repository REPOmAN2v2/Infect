#ifndef INCLUDE_H_
#define INCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

typedef enum _directions {NORTH, WEST, SOUTH, EAST} Directions;
typedef enum _deltas {NONE, N=-1, W=-1, S=1, E=1} Deltas;
typedef enum _characters {EMPTY,DEAD, INF, DOC, CIT, SOL, NUR, WALL} Characters;

typedef struct _board {
	Characters character;
	int direction;
} Board;

extern int countDoc, countInf, countNur, countSol, 
				countCit, countDea, elapsed, total, countWood;
extern const int localTimeout;
extern int X, Y, refreshRate, days, stepthrough;

void checkWin(Board **board);
void win(Board **board, int);

#endif