#include <time.h>
#include <string.h>

#include "display.h"
#include "gameplay.h"
#include "generation.h"

unsigned int 	countDoc = 0, countInf = 0,  countNur = 0, countSol = 0, 
				countCit = 0, countDea = 0, countWood = 0, elapsed = 0, total = 0;
const unsigned int localTimeout = 200000;
unsigned int refreshRate = 5, days = 0;

int main (int argc, char **argv) 
{
	srand(time(NULL));

	Board **board = parseArgs(argc, argv);
	initNcurses();

	do {
		if (!(days % refreshRate)) {
			clear();
			displayBoard(board);
			refresh();
			sleep(1);
		}
		checkWin(board);
		getActions(board);
		getMoves(board);
		++days;
		if (stepthrough) {
			while (getch() != 'n');
		}
	} while (1);	
}

void checkWin(Board **board) 
{
	if (countInf >= (countCit + countNur + countDoc + countSol)*25) {
		win(board, 0);
	} else if (countSol >= (countCit + countNur + countDoc)*1.5 && countSol >= countInf) {
		win(board, 3);
	} else if (elapsed >= localTimeout) {
		win(board, 1);
	} else if (!countInf) {
		win(board, 2);
	}
}
	
void win(Board **board, int outcome)
{
	int pos = displayBoard(board);

	if (outcome == 2) {
		mvprintw(++pos, 0, "It only took %u days for the infection to be eliminated\n", days);
	} else if (outcome == 1) {
		mvprintw(++pos, 0, "It only took %u days for the infection to be contained\n", days);
	} else if (outcome == 3) {
		mvprintw(++pos, 0, "It only took %u days for a military dictatorship to be established\n", days);
	} else {
		mvprintw(++pos, 0, "It only took %u days for the world to descend into chaos\n", days);
	}

	//mvprintw(++pos, 0, "Doctors: %u - Infected: %u - Citizens: %u - Nurses: %u - Soldiers: %u - Dead: %u  - Days: %u\n", 
	//countDoc, countInf, countCit, countNur, countSol, countDea, days);
	mvprintw(++pos, 0, "Press q to quit\n");

	refresh();
	while(getch() != 'q');
	endwin();
	for(size_t i = 0; i < Y; i++)
		free(board[i]);
	free(board);

	exit(EXIT_SUCCESS);
}