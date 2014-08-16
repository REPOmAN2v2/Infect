#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "include.h"

typedef void (*action)(Board*, Board*);

void getMoves(Board **);
void getActions(Board **);
Board * getDelta(Board **, int, int);
void checkTarget(Board **, int, int, action);
void checkSoldierRadius(Board **, int, int);
int canShoot(Board **board, int x, int y);
void getActionInf(Board *, Board *);
void getActionDoc(Board *, Board *);
void getActionCit(Board *, Board *);
void getActionSol(Board **, Board *, Board *, int, int);
void getActionNurse(Board *, Board *);
int checkOutBounds(Board **, Directions, int, int);

#endif