#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define X 100
#define Y 50

typedef enum _directions {NORTH, WEST, SOUTH, EAST} Directions;
typedef enum _deltas {NONE, N=-1, W=-1, S=1, E=1} Deltas;
typedef enum _characters {EMPTY,DEAD, INF, DOC, CIT, SOL, NUR} Characters;

typedef struct _board {
	Characters character;
	int direction;
} Board;

typedef void (*action)(Board*, Board*);

unsigned int 	countDoc = 0, countInf = 0,  countNur = 0, countSol = 0, 
				countCit = 0, countDea = 0, elapsed = 0;
const unsigned int localTimeout = 200000;

void generateCoord(Board board[][X], const int count, Characters type);
void initialise(Board board[][X]);
void defaultBoard(Board board[][X]);
int displayBoard(Board board[][X], int days);
void getActions(Board board[][X]);
Board * getDelta(Board board[][X], int y, int x);
void getMoves(Board board[][X]);
void checkTarget(Board board[][X], int y, int x, action getAction);
void checkSoldierRadius(Board board[][X], int y, int x);
void getActionInf(Board *infected, Board *target);
void getActionDoc(Board *doctor, Board *target);
void getActionCit(Board *soldier, Board *target);
void getActionSol(Board board[][X], Board *soldier, Board *target, int x, int y);
void getActionNurse(Board *nurse, Board *target);
int checkOutBounds(Board board[][X], Directions move, int y, int x);
void checkWin(Board board[][X], unsigned int days);
void win(Board board[][X], int outcome, unsigned int days);

int main (int argc, char **argv) 
{
	unsigned int days = 0;
	srand(time(NULL));
	initscr();
	noecho();
	start_color();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);

	Board board[Y][X] = {{0}};	

	initialise(board);
	do {
		if (!(days%5)) {
			displayBoard(board, days);
			refresh();
			sleep(1);
		}
		checkWin(board, days);
		getActions(board);
		getMoves(board);
		++days;
	} while (1);	
}

void initialise(Board board[][X])
{
	countDoc = 10, countSol = 1, countInf = 3;
	countCit = X*Y - countDoc - countSol - countInf;

	defaultBoard(board);	
	generateCoord(board, countSol, SOL);
	generateCoord(board, countInf, INF);
	generateCoord(board, countDoc, DOC);
}

void getMoves(Board board[][X])
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

void getActions(Board board[][X]) 
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

Board * getDelta(Board board[][X], int y, int x)
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

void checkTarget(Board board[][X], int y, int x, action getAction)
{
	Board *target = NULL;
	
	target = getDelta(board, y, x);

	if (target == NULL) exit(EXIT_FAILURE);

	getAction(&board[y][x], target);
}

void checkSoldierRadius(Board board[][X], int y, int x)
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

	if (x >= 0 && x < X && y >=0 && y < Y) {
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
			infected->character = DEAD;
			target->character = DEAD;
		} else if (prob < 50) {
			target->character = INF;
			++countInf;
		} else if (prob < 75) {
			if (target->character == DOC) --countDoc;
			else --countNur;
			target->character = DEAD;
		} else {
			infected->character = DEAD;
			--countInf;
		}

		elapsed = 0;

	} else if (target->character == SOL) {
		if (rand()%100 < 45) {
			target->character = DEAD;
			--countSol;
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

void getActionSol(Board board[][X], Board *soldier, Board *target, int x, int y)
{
	int prob = rand()%100;
	if (prob < 2 && (target->character == DOC || target->character == NUR)) {
		prob = rand()%100;
	} 

	if (prob < 25) {

		int shoot = 0;
		for (int i = y - 2; i < y + 3; i++) {
			for (int j = x - 3; j < x + 4; j++) {
				if (j >= 0 && j < X && i >=0 && i < Y) {
					if (board[i][j].character == CIT) {
						shoot = 1;
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

int checkOutBounds(Board board[][X], Directions move, int y, int x)
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

int displayBoard(Board board[][X], int days) 
{
	size_t i;
	
	for (i = 0; i < Y; i++) {
		for (size_t j = 0; j < X; j++) {
			switch (board[i][j].character) {
				case SOL:
					attron(COLOR_PAIR(4));
					mvprintw(i, j, "S");
					attroff(COLOR_PAIR(4));
					break;
				
				case INF:
					attron(COLOR_PAIR(1));
					mvprintw(i, j, "I");
					attroff(COLOR_PAIR(1));
					break;

				case DOC:
					attron(COLOR_PAIR(3));
					mvprintw(i, j, "D");
					attroff(COLOR_PAIR(3));
					break;

				case CIT:
					attron(COLOR_PAIR(2));
					mvprintw(i, j, "O");
					attroff(COLOR_PAIR(2));		
					break;

				case NUR:
					attron(COLOR_PAIR(5));
					mvprintw(i, j, "N");
					attroff(COLOR_PAIR(5));
					break;

				case DEAD:
					mvprintw(i, j, "X");
					break;
				
				case EMPTY:
					mvprintw(i, j, " ");
					break;
			}
		}
	}
	mvprintw(i, 0, "Day %d", days);

	return i;
}	

void defaultBoard(Board board[][X])
{
	for (size_t i = 0; i < Y; i++) {
		for (size_t j = 0; j < X; j++) {
			board[i][j].character = CIT;
			board[i][j].direction = NONE;
		}
	} 
}

void generateCoord(Board board[][X], const int count, Characters type)
{
	int x = 0, y = 0;

	for (size_t i = 0; i < count; i++) {
		do {
			x = rand()%X;
			y = rand()%Y;
		} while(board[y][x].character != CIT);
		
		board[y][x].character = type;
	} 
}

void checkWin(Board board[][X], unsigned int days) 
{
	if (countInf >= X*Y*0.8) {
		win(board, 0, days);
	}
	if (elapsed >= localTimeout || !countInf) {
		win(board, 1, days);
	}
}
	
void win(Board board[][X], int outcome, unsigned int days)
{
	int pos = displayBoard(board, days);

	if (outcome == 1) {
		if (!countInf) {
			mvprintw(++pos, 0, "It only took %u days for the infection to be eliminated\n", days);
		} else {
			mvprintw(++pos, 0, "It only took %u days for the infection to be contained\n", days);
		}
	} else {
		mvprintw(++pos, 0, "It only took %u days for the world to descend into chaos\n", days);
	}

	mvprintw(++pos, 0, "Doctors: %u - Infected: %u - Citizens: %u - Nurses: %u - Soldiers: %u - Dead: %u  - Days: %u\n", countDoc, countInf, 
		countCit, countNur, countSol, countDea, days);

	refresh();
	while(1);
	endwin();
	exit(EXIT_SUCCESS);
}