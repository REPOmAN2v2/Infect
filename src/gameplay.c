#include "gameplay.h"

void getMoves(Board **board)
{
	for (size_t i = 0; i < Y; i++) {
		for (size_t j = 0; j < X; j++) {
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
				case DEAD:
				case EMPTY:
					break;
			}
		}
	}
}

void getActions(Board **board) 
{
	for (size_t i = 0; i < Y; i++) {
		for (size_t j = 0; j < X; j++) {
			++elapsed;
			board[i][j].direction = rand()%4; 
			switch (board[i][j].character) {
				case DEAD: //fallthrough
				case EMPTY:
					break;
				case INF:
					if (rand()%100 >= 75 && checkOutBounds(board, board[i][j].direction, i, j)) { 
						checkTarget(board, i, j, getActionInf);
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

	if (xt >= 0 && xt < X && yt >=0 && yt < Y) {
		Board *attacker = &board[y][x];
		Board *target = &board[yt][xt];
		getActionSol(board, attacker, target, x, y);
	}
}

void getActionInf(Board *infected,  Board *target)
{
	if (target->character == CIT) {
		target->character = INF;
		elapsed = 0;
		--countCit;
		++countInf;
	} else if (target->character == DOC || target->character == NUR) {
		int prob = rand()%100;
	
		if (prob < 5) {
			infected->character = NUR;
			--countInf;
			++countNur;
		} else if (prob < 10) {
			infected->character = CIT;
			--countInf;
			++countCit;
		} else if (prob < 15) {
			infected->character = SOL;
			--countInf;
			++countSol;
		} else if (prob < 25) {
			--countInf;
			if (target->character == DOC) --countDoc;
			else --countNur;
			countDea += 2;

			infected->character = DEAD;
			target->character = DEAD;
		} else if (prob < 50) {
			target->character = INF;
			++countInf;
		} else if (prob < 75) {
			if (target->character == DOC) --countDoc;
			else --countNur;
			++countDea;

			target->character = DEAD;
		} else {
			infected->character = DEAD;
			--countInf;
			++countDea;
		}

		elapsed = 0;

	} else if (target->character == SOL) {
		if (rand()%100 < 45) {
			target->character = DEAD;
			--countSol;
			++countDea;
		}

	}		
}

void getActionDoc(Board *doctor, Board *target)
{
	int prob = rand()%100;
	if (target->character == CIT || target->character == INF) {
		if (prob < 7) {
			++countNur;
			if (target->character == CIT) --countCit;
			else --countInf;
			target->character = NUR;
			elapsed = 0;
		}
	} else if (target->character == INF) {
		if (prob < 17) {
			target->character = CIT;
			++countCit;
			--countInf;
			elapsed = 0;
		}
	} else if (target->character == DEAD) {
		if (prob == 10) {
			target->character = CIT;
			++countCit;
			--countDea;
			elapsed = 0;
		}
	}
}

void getActionCit(Board *citizen, Board *target)
{
	int prob = rand()%100;
	if (prob == 10) {
		citizen->character = DOC;
		--countCit;
		++countDoc;
		elapsed = 0;
	} else if (prob == 23) {
		citizen->character = INF;
		--countCit;
		++countDoc;
		elapsed = 0;
	}
}

void getActionSol(Board **board, Board *soldier, Board *target, int x, int y)
{
	int prob = rand()%100;
	if (prob < 2 && (target->character == DOC || target->character == NUR)) {
		prob = rand()%100;
	} 

	if (prob < 25) {

		int shoot = 1;
		for (int i = y - 2; i < y + 3; i++) {
			for (int j = x - 3; j < x + 4; j++) {
				if (j >= 0 && j < X && i >=0 && i < Y) {
					if (board[i][j].character == CIT) {
						shoot = 0;
					}
				}
			}
		}

		if (shoot) {
			if (target->character == INF) {
				target->character = DEAD;
				++countDea;
				--countInf;
				elapsed = 0;
			} else if (prob < 2 && (target->character == NUR || target->character == SOL)) {
				if (target->character == NUR) --countNur;
				else --countSol;
				++countDea;
				target->character = DEAD;
				elapsed = 0;
			}
		}

		if (target->character == CIT) {
			if (prob < 20) {
				target->character = SOL;
				--countCit;
				++countSol;
				elapsed = 0;
			}
		} else if (target->character == DEAD) {
			target->character = EMPTY;
			--countDea;
		}
	}
}

void getActionNurse(Board *nurse, Board *target)
{
	int prob = rand()%100;
	if (target->character == INF) {
		if (prob < 3) {
			target->character = NUR;
			--countInf;
			++countNur;
		} else if (prob < 17) {
			target->character = CIT;
			--countInf;
			++countCit;
		}
	}
}

int checkOutBounds(Board **board, Directions move, int y, int x)
{
	if (move == NORTH) {		
		return (y + N >= 0);
	} else if (move == SOUTH) {
		return (y + S < Y);
	} else if (move == WEST) {
		return (x + W >= 0);
	} else {
		return (x + E < X);
	}
}