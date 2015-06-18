#include "style.hpp"
#include "ncurses.hpp"

Style::Style(int h, int w)
{
	int cur_w, cur_h, y , x;
	getmaxyx(stdscr, cur_h, cur_w);

	if (h == -1) {
		h = cur_h;
	} else {
		y = cur_h/2 - h/2;
	}

	if (w == -1) {
		w = cur_w;
	} else {
		x = cur_w/2 - w/2;
	}

	main = new Window(h, w, y, x);

	main->refresh();
}

Style::~Style()
{
	if (main) {
		delete main;
	}
}