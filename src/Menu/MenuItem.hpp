#pragma once

#include "window.hpp"
#include <string>

class MenuItem
{
public:
	MenuItem(std::string label, int id);
	void draw(Window *window, bool cur, int w, int y, int x);
	std::string label;
	int id;
};