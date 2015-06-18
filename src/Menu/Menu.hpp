#pragma once

#include "MenuStyle.hpp"
#include "MenuData.hpp"

class Menu
{
public:
	Menu();
	~Menu();
	void createMainMenu();
	void draw();
private:
	MenuStyle *style;
	MenuData *data;
};