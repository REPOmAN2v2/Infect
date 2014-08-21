#include <time.h>
#include <string.h>

#include "display.h"
#include "gameplay.h"
#include "generation.h"
#include "gamemenu.h"

static void checkWin(Board **board);
static void win(Board **board, int);

Variables gameVar = {{0}, {79, 20}, {0, 20000, 0, FAST, 0}};

int main (int argc, char **argv) 
{
	srand(time(NULL));

	Board **board = NULL;

	if (argc == 1) {
		displayMenu();
		board = initDefault();
	} else {
		board = parseArgs(argc, argv);
	}

	initNcurses();
	Time *times = &gameVar.time;

	do {
		if (!(times->days % times->refreshRate)) {
			clear();
			displayBoard(board);
			refresh();
			sleep(1);
		}
		checkWin(board);
		getActions(board);
		getMoves(board);
		++times->days;
		if (times->steps) {
			while (getch() != 'n');
		}
	} while (1);	
}

void checkWin(Board **board) 
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	if (units->infected >= (units->citizens + units->nurses + units->doctors + units->soldiers)*25) {
		win(board, 0);
	} else if (units->soldiers >= (units->citizens + units->nurses + units->doctors)*1.5 && units->soldiers >= units->infected) {
		win(board, 3);
	} else if (times->elapsed >= times->timeout) {
		win(board, 1);
	} else if (!units->infected) {
		win(board, 2);
	}
}
	
void win(Board **board, int outcome)
{
	int pos = displayBoard(board);
	Time *times = &gameVar.time;

	if (outcome == 2) {
		mvprintw(++pos, 0, "It only took %u days for the infection to be eliminated\n", times->days);
	} else if (outcome == 1) {
		mvprintw(++pos, 0, "It only took %u days for the infection to be contained\n", times->days);
	} else if (outcome == 3) {
		mvprintw(++pos, 0, "It only took %u days for a military dictatorship to be established\n", times->days);
	} else {
		mvprintw(++pos, 0, "It only took %u days for the world to descend into chaos\n", times->days);
	}

	//mvprintw(++pos, 0, "Doctors: %u - Infected: %u - Citizens: %u - Nurses: %u - Soldiers: %u - Dead: %u  - Days: %u\n", 
	//countDoc, countInf, countCit, countNur, countSol, countDea, days);
	mvprintw(++pos, 0, "Press q to quit\n");

	refresh();
	while(getch() != 'q');
	endwin();
	for(size_t i = 0; i < gameVar.dim.y; i++)
		free(board[i]);
	free(board);

	exit(EXIT_SUCCESS);
}