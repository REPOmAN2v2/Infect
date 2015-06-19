#include "style.hpp"
#include "ncurses.hpp"

Style::Style(int h, int w)
{
	int y, x;

	if (h == -1) {
		h = LINES;
	}

	if (w == -1) {
		w = COLS;
	}

	y = (LINES - h) / 2;
	x = (COLS - w) / 2;

	main = new Window(h, w, y, x);
}

Style::~Style()
{
	if (main) {
		delete main;
	}
}