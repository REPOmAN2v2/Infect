#include "display.h"
#include <string.h>

void initNcurses()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	start_color();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_GREEN);
}

int displayBoard(Board **board) 
{
	Units *units = &gameVar.units;
	size_t i;
	
	for (i = 0; i < gameVar.dim.y; i++) {
		for (size_t j = 0; j < gameVar.dim.x; j++) {
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

				case WALL:
					attron(COLOR_PAIR(6));
					mvprintw(i, j, "W");
					attroff(COLOR_PAIR(6));
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
	mvprintw(i, 0, "Day %u", gameVar.time.days);
	mvprintw(++i, 0, "Citizens: %u, Infected: %u, Doctors: %u, Nurses: %u, Soldiers: %u, Dead: %u", 
		units->citizens, units->infected, units->doctors, units->nurses, units->soldiers, units->dead);

	return i;
}

void printError(const char *error)
{
	endwin();
	fprintf(stderr, error);
	printHelp();
}

void printHelp()
{
	fprintf(stdout, "--map/-m <str>\t\tSpecify a file to read from containing a map, incompatible with -x/-y\n");
    fprintf(stdout, "-x <int>\t\tUse in conjunction with --y <int> to specify dimensions of auto-generated map\n");
    fprintf(stdout, "-y <int>\t\tSee above\n");
    fprintf(stdout, "-d/-i/-s/-n/-w <int>\tSpecify the number of doctors/infected/soldiers/nurses/wood.\n");
    fprintf(stdout, "--slow\t\t\tRun the simulation with slow speed. Very slow.\n");
    fprintf(stdout, "--fast\t\t\tRun the simulation with fast speed. Almost real-time!\n");
    fprintf(stdout, "--fastest\t\tRun the simulation at fastest speed.\n");
    fprintf(stdout, "--version/-v\t\tPrint version information.\n");
    fprintf(stdout, "Default values are x=79, y=20, fast speed, 16 doctors, 16 infected, 32 soldiers, 80 nurses and 790 wood. They scale with x and y.\n");
    exit(EXIT_SUCCESS);
}

void printVersion()
{
	fprintf(stdout, "Infect 1.1\n");
	fprintf(stdout, "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
	fprintf(stdout, "This is free software: you are free to change and redistribute it.\n");
	fprintf(stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
	exit(EXIT_SUCCESS);
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
	int length, x, y;
	float temp;

	if (win == NULL) {
		win = stdscr;
	}

	getyx(win, y, x);
	if (startx != 0) {
		x = startx;
	}

	if (starty != 0) {
		y = starty;
	}

	if (width == 0) {
		width = 80;
	}

	length = strlen(string);
	temp = (width - length) / 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}