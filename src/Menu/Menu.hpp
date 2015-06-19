#pragma once

#include "MenuStyle.hpp"
#include "MenuData.hpp"
#include "MenuItemTemplates.hpp"

class Menu
{
public:
	Menu();
	~Menu();
	void createMainMenu();
	void draw();
	void update();
private:
	void createMenu(std::vector<MenuItemTemplate> &itemTemplate);
	MenuStyle *style;
	MenuData *data;
};