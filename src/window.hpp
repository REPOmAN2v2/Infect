#pragma once

#include <ncurses.h>
#include <string>

class Window
{
public:
	Window(int h, int w, int y, int x);
	// Subwindows need a ref to the parent
	Window(Window *parent, int h, int w, int y, int x);
	virtual ~Window() {};
	void resize(int h, int w);
	void refresh();
	void clear();
	void setTitle();
	void setBorders();
	void print(std::string line, int y, int x);
	int getH(){return _h;};
	int getW(){return _w;};
	int getY(){return _y;};
	int getX(){return _x;};
private:
	WINDOW *win;
	int _h, _w, _y, _x;
};