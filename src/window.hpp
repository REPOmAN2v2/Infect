#pragma once

#include <ncurses.h>

class Window
{
public:
	Window(int x, int y, int w, int h);
	~Window();
	void resize();
	void refresh();
	void clear();
	void setTitle();
private:
	WINDOW *win;
	int _x, _y, _w, _h;
}