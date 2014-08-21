#include "gameplay.h"

/*
 * Internal constants and definitions
 */

 typedef enum _directions {NORTH, WEST, SOUTH, EAST} Directions;
 typedef enum _deltas {NONE, N=-1, W=-1, S=1, E=1} Deltas;

/*
 * Internal prototypes
 */

typedef void (*action)(Board*, Board*);

static Board * getDelta(Board **board, int y, int x);
static void checkTarget(Board **board, int y, int x, action getAction);
static void checkSoldierRadius(Board **board, int y, int x);
static int canShoot(Board **board, int x, int y);
static void getActionInf(Board *infected, Board *target);
static void getActionDoc(Board *doctor, Board *target);
static void getActionCit(Board *citizen, Board *target);
static void getActionSol(Board **board, Board *soldier, Board *target, int y, int x);
static void getActionNurse(Board *nurse, Board *target);
static int checkOutBounds(Board **board, Directions move, int y, int x);

void getMoves(Board **board)
{
	for (size_t i = 0; i < gameVar.dim.y; i++) {
		for (size_t j = 0; j < gameVar.dim.x; j++) {
			switch (board[i][j].character) {
				case INF:
				case DOC:
				case CIT:
				case SOL:
				case NUR:
					board[i][j].direction = rand()%4;
					if (checkOutBounds(board, board[i][j].direction, i, j)) {
						Board *target = getDelta(board, i, j);
						if (target->character == EMPTY) {
							target->character = board[i][j].character;
							board[i][j].character = EMPTY;
						}
					}
					break;
				case DEAD:	//fallthrough
				case WALL:
				case EMPTY:
					break;
			}
		}
	}
}

void getActions(Board **board) 
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	for (size_t i = 0; i < gameVar.dim.y; i++) {
		for (size_t j = 0; j < gameVar.dim.x; j++) {
			++times->elapsed;
			board[i][j].direction = rand()%4; 
			switch (board[i][j].character) {
				case WALL:	//fallthrough
				case EMPTY:
					break;
				case DEAD:
					if (rand()%100 == 1) {
						board[i][j].character = EMPTY;
						--units->dead;
					}
				case INF:
					for (board[i][j].direction = 0; board[i][j].direction < 4; board[i][j].direction++) {
						if (checkOutBounds(board, board[i][j].direction, i, j)) { 
							checkTarget(board, i, j, getActionInf);
						}
					}
					break;
				case DOC:
					if (checkOutBounds(board, board[i][j].direction, i, j)) {
						checkTarget(board, i, j, getActionDoc);	
					}
					break;
				case CIT:
					if (rand()%100 >= 98 && checkOutBounds(board, board[i][j].direction, i, j)) {
						checkTarget(board, i, j, getActionCit);
					}
					break;
				case SOL:
					checkSoldierRadius(board, i, j);
					break;
				case NUR:
					if (checkOutBounds(board, board[i][j].direction, i, j)) {
						checkTarget(board, i, j, getActionNurse);
					}
			}
		}
	}
}

Board * getDelta(Board **board, int y, int x)
{
	Board *attacker = &board[y][x];
	Board *target = malloc(sizeof(Board));	
	Deltas delta = attacker->direction == NORTH ? N : attacker->direction == SOUTH ? S : attacker->direction == WEST ? W : E;

	if (attacker->direction == NORTH || attacker->direction == SOUTH) {
		target = &board[y + delta][x];
	} else {
		target = &board[y][x + delta];
	}

	return target;

} 

void checkTarget(Board **board, int y, int x, action getAction)
{
	Board *target = NULL;
	
	target = getDelta(board, y, x);

	if (target == NULL) exit(EXIT_FAILURE);

	getAction(&board[y][x], target);
}

void checkSoldierRadius(Board **board, int y, int x)
{
	int r1 = rand()%2, r2 = rand()%3;
	int xt = x, yt = y;

	if (rand()%2) {
		yt += r1;
	} else {
		yt -= r1;
	}

	if (rand()%2) {
		xt =+ r2;
	} else {
		xt -= r2;
	}

	if (xt >= 0 && xt < gameVar.dim.x && yt >=0 && yt < gameVar.dim.y) {
		Board *attacker = &board[y][x];
		Board *target = &board[yt][xt];
		getActionSol(board, attacker, target, y, x);
	}
}

void getActionInf(Board *infected,  Board *target)
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	int prob = rand()%100;

	if (rand()%100 >= 95) {
		if (target->character == CIT) {
			--units->citizens;
			++units->infected;

			target->character = INF;
			times->elapsed = 0;
		} else if (target->character == WALL && prob < 5) {
			target->character = EMPTY;
			units->wood += (rand()%25)+1;
		} else if (target->character == DOC || target->character == NUR) {
			if (prob < 25) {
				if (target->character == DOC) --units->doctors;
				else --units->nurses;
				++units->dead;

				target->character = DEAD;
			} else if (prob < 30) {
				--units->infected;
				++units->dead;

				infected->character = DEAD;
			} else if (prob < 60) {
				if (target->character == DOC) --units->doctors;
				else --units->nurses;
				++units->infected;

				target->character = INF;
			} else if (prob < 70) {
				--units->infected;
				++units->citizens;

				infected->character = CIT;
			} else if (prob < 100) {
				if (target->character == DOC) --units->doctors;
				else --units->nurses;
				--units->infected;
				units->dead += 2;

				infected->character = DEAD;
				target->character = DEAD;
			}
		} else if (target->character == SOL) {
			if (prob < 10) {
				--units->infected;
				++units->dead;

				infected->character = DEAD;
			} else if (prob < 60) {
				--units->soldiers;
				++units->dead;

				target->character = INF;
				infected->character = DEAD;
			} else if (prob < 90) {
				--units->soldiers;
				++units->infected;

				target->character = INF;
			} else {
				--units->soldiers;
				++units->dead;

				target->character = DEAD;
			}
		}
		times->elapsed = 0;
	}
}

void getActionDoc(Board *doctor, Board *target)
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	int prob = rand()%100;
	if (prob < 20) {
		if (target->character == CIT) {
			--units->citizens;
			++units->nurses;

			target->character = NUR;
		} else if (target->character == NUR) {
			--units->nurses;
			++units->doctors;

			target->character = DOC;
		}
	} else if (target->character == INF) {
		if (prob < 25) {
			++units->citizens;
			--units->infected;

			target->character = CIT;
		}
	} else if (target->character == DEAD) {
		if (prob == 10) {
			++units->citizens;
			--units->dead;

			target->character = CIT;
		}
	}

	times->elapsed = 0;
}

void getActionCit(Board *citizen, Board *target)
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	int prob = rand()%100;
	if (prob == 20) {
		--units->citizens;
		++units->soldiers;

		citizen->character = SOL;
	} else if (prob < 15) {
		if (target->character == EMPTY && times->days >= 100 && units->wood >= 25) {
			target->character = WALL;
			units->wood -= (rand()%25)+1;
			times->elapsed = 0;
		}
	}
}

void getActionSol(Board **board, Board *soldier, Board *target, int y, int x)
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	int prob = rand()%100;
	if (prob < 2 && (target->character == DOC || target->character == NUR)) {
		prob = rand()%100;
	} 

	if (prob < 80) { 
		if (canShoot(board, y, x)) {
			if (target->character == INF) {
				++units->dead;
				--units->infected;

				target->character = DEAD;
				times->elapsed = 0;
			} else if (prob < 2 && (target->character == NUR || target->character == DOC)) {
				if (target->character == NUR) --units->nurses;
				else --units->doctors;
				++units->dead;

				target->character = DEAD;
				times->elapsed = 0;
			}
		}

		if (target->character == CIT) {
			if (prob >= 80) {
				--units->citizens;
				++units->soldiers;

				target->character = SOL;
				times->elapsed = 0;
			}
		} else if (target->character == DEAD) {
			--units->dead;

			target->character = EMPTY;
		}
	}
}

int canShoot(Board **board, int y, int x)
{
	for (int i = y - 2; i < y + 3; i++) {
		for (int j = x - 3; j < x + 4; j++) {
			if (j >= 0 && j < gameVar.dim.x && i >=0 && i < gameVar.dim.y) {
				if (board[i][j].character == CIT) {
					return 0;
				}
			}
		}
	}

	return 1;
}

void getActionNurse(Board *nurse, Board *target)
{
	Units *units = &gameVar.units;

	int prob = rand()%100;
	if (target->character == INF) {
		/*if (prob < 3) {
			--countInf;
			++countNur;

			target->character = NUR;
		} else*/ if (prob < 20) {
			--units->infected;
			++units->citizens;

			target->character = CIT;
		}
	}
}

int checkOutBounds(Board **board, Directions move, int y, int x)
{
	if (move == NORTH) {		
		return (y + N >= 0);
	} else if (move == SOUTH) {
		return (y + S < gameVar.dim.y);
	} else if (move == WEST) {
		return (x + W >= 0);
	} else {
		return (x + E < gameVar.dim.x);
	}
}