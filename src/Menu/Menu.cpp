#include "Menu.hpp"
#include "MenuItemTemplates.hpp"

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

	size_t sz = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);
	MainMenu *end = &mainMenuItems[sz - 1];

	for (MainMenu *it = mainMenuItems; it <= end; ++it) {
		if (it->name) {
			MenuItem *item = new MenuItem(it->name, it->id);
			data->addItem(item);
		} else {
			data->addItem(nullptr);
		}
	}
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