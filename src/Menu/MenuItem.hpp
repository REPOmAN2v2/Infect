#pragma once

#include "window.hpp"
#include "MenuItemTemplates.hpp"
#include <string>

class MenuItem
{
public:
	MenuItem(MenuItemTemplate &tem);
	virtual ~MenuItem() {};
	virtual void draw(Window *window, bool cur, int w, int y, int x);
	virtual void update(int key) {};
	ID id;
	Type type;
protected:
	std::string label;
};