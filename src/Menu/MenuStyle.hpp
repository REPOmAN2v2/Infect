#pragma once

#include "style.hpp"
#include "window.hpp"
#include "MenuData.hpp"

class MenuStyle : public Style
{
public:
	MenuStyle(int h, int w);
	~MenuStyle();
	void draw(MenuData *data);
	Window *menu;
private:
};