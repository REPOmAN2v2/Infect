#pragma once

#include "window.hpp"
#include "MenuItem.hpp"
#include <vector>

class MenuData
{
public:
	MenuData(int h, int w, int y, int x);
	~MenuData();
	void addItem(MenuItem *item);
	void removeItem(ID id);
	void draw(Window *window);
	void nextItem();
	void prevItem();
	void firstItem();
	void lastItem();
	void update();
	ID whichSelected();
private:
	int _h, _w, _y, _x;
	std::vector<MenuItem*> items;
	MenuItem *current, *selected;
	size_t index;
};