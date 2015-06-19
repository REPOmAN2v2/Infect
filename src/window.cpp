#include "window.hpp"

Window::Window(int h, int w, int y, int x):_h(h),_w(w),_y(y),_x(x)
{
	win = newwin(h, w, y, x);
}

Window::Window(Window *parent, int h, int w, int y, int x):_h(h),_w(w),_y(y),_x(x)
{
	if (w == -1) {
		_w = parent->_w; 
	}

	if (h == -1) {
		_h = parent->_h;
	}

	win = derwin(parent->win, _h, _w, y, x);
}

void Window::resize(int h, int w)
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

void Window::print(std::string line, int y, int x)
{
	//wattrset(win, COLOR_PAIR(7));
	if (!line.empty()) {
		mvwaddstr(win, y, x, line.c_str());
	}
}

void Window::setBorders()
{
	box(win, 0, 0);
}