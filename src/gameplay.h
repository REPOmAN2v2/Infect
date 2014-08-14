#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "include.h"

typedef void (*action)(Board*, Board*);

void getMoves(Board board[][X]);
void getActions(Board board[][X]);
Board * getDelta(Board board[][X], int, int);
void checkTarget(Board board[][X], int, int, action);
void checkSoldierRadius(Board board[][X], int, int);
void getActionInf(Board *, Board *);
void getActionDoc(Board *, Board *);
void getActionCit(Board *, Board *);
void getActionSol(Board board[][X], Board *, Board *, int, int);
void getActionNurse(Board *, Board *);
int checkOutBounds(Board board[][X], Directions, int, int);

#endif