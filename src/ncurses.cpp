#include "ncurses.hpp"
#include <ncurses.h>

bool Ncurses::init()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	/*start_color();

	for (int i = COLOR_BLACK; i <= COLOR_WHITE; ++i) {
		init_pair(i, i, -1);
	}*/

	refresh();

	return true; // TODO: error handling
}

void Ncurses::exit()
{
	erase();
	refresh();
	endwin();
}