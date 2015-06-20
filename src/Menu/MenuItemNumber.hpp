#pragma once

#include "MenuItem.hpp"

class MenuItemNumber: public MenuItem
{
public:
	MenuItemNumber(MenuItemTemplate &tem);

	void draw(Window *window, bool cur, int w, int y, int x);
	void update(int key);

private:
	int min, max, start, current;
};