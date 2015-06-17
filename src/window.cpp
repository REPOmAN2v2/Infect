#include "window.hpp"

Window::Window(int x, int y, int w, int h):_x(x),_y(y),_w(w),_h(h)
{
	win = newwin(h, w, y, x);
}

void Window::resize(int w, int h)
{
	wresize(win, h, w);
	_w = w;
	_h = h;
}

void Window::refresh()
{
	wrefresh(win);
}

void Window::clear()
{
	werase(win);
}