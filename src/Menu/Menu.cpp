#include "Menu.hpp"

enum Options {PLAY, SETTINGS, HELP, QUIT};

Menu::Menu():data(nullptr)
{
	style = new MenuStyle(20, 79);
}

void Menu::createMainMenu()
{
	if (data) {
		delete data;
	}

	data = new MenuData(style->menu->getH() - 2,
						style->menu->getW() - 2,
						1,1);

	MenuItem *item = new MenuItem("Play", PLAY);
	data->addItem(item);

	item = new MenuItem("Settings", SETTINGS);
	data->addItem(item);

	item = new MenuItem("Help", HELP);
	data->addItem(item);

	data->addItem(nullptr);

	item = new MenuItem("Quit", QUIT);
	data->addItem(item);

	item = new MenuItem("Test1", QUIT+1);
	data->addItem(item);
	item = new MenuItem("Test2", QUIT+2);
	data->addItem(item);
	item = new MenuItem("Test3", QUIT+3);
	data->addItem(item);
	item = new MenuItem("Test4", QUIT+4);
	data->addItem(item);
}

Menu::~Menu()
{
	if (style) {
		delete style;
	}

	if (data) {
		delete data;
	}
}

void Menu::draw()
{
	style->draw(data);
}

void Menu::update()
{
	data->update();
}