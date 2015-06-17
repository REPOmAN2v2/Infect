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

	return true; // TODO: error handling
}

void Ncurses::exit()
{
	erase();
	refresh();
	endwin();
}