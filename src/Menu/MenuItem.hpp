#pragma once

#include "window.hpp"
#include "MenuItemTemplates.hpp"
#include <string>

class MenuItem
{
public:
	MenuItem(std::string label, ID id);
	void draw(Window *window, bool cur, int w, int y, int x);
	std::string label;
	ID id;
};