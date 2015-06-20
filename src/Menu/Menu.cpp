#include "Menu.hpp"
#include <vector>

// TODO: move these to a config file (JSON?)
static std::vector<MenuItemTemplate> mainMenuItems = {
	{.name = "Play", .id = ID::PLAYMENU, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0},
	{.name = "Settings", .id = ID::SETTINGS, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0},
	{.name = "Help", .id = ID::HELP, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0},
	{.name = "Quit", .id = ID::QUIT, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0}
};

static std::vector<MenuItemTemplate> playMenuItems = {
	{.name = "Launch game", .id = ID::PLAY, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0},
	{.name = "Back", .id = ID::BACK, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0},
	{.name = nullptr, .id = ID::NONE, .type = Type::NONE, .min = 0, .max = 0, .start = 0},
	{.name = "Doctors", .id = ID::DOCTORS, .type = Type::NUMBER, .min = 0, .max = 100, .start = 5},
	{.name = "Infected", .id = ID::INFECTED, .type = Type::NUMBER, .min = 0, .max = 100, .start = 8},
	{.name = "Nurses", .id = ID::NURSES, .type = Type::NUMBER, .min = 0, .max = 100, .start = 6},
	{.name = "Soldiers", .id = ID::SOLDIERS, .type = Type::NUMBER, .min = 0, .max = 100, .start = 10},
	{.name = "Lumber", .id = ID::LUMBER, .type = Type::NUMBER, .min = 0, .max = 1000, .start = 50},
	{.name = nullptr, .id = ID::NONE, .type = Type::NONE, .min = 0, .max = 0, .start = 0},
	{.name = "Sim speed", .id = ID::SPEED, .type = Type::TEXT, .min = 0, .max = 0, .start = 0},
	{.name = "Step", .id = ID::STEP, .type = Type::TOGGLE, .min = 0, .max = 0, .start = 0},
	{.name = "Reset", .id = ID::RESET, .type = Type::SIMPLE, .min = 0, .max = 0, .start = 0}
};

Menu::Menu():data(nullptr)
{
	style = new MenuStyle(20, 79);
}

void Menu::createMainMenu()
{
	createMenu(mainMenuItems);
}

void Menu::createMenu(std::vector<MenuItemTemplate> &itemTemplate)
{
	if (data) {
		delete data;
	}

	data = new MenuData(style->menu->getH() - 2,
						style->menu->getW() - 2,
						1,1);

	for (auto it : itemTemplate) {
		if (it.name) {
			MenuItem *item;

			if (it.type == Type::NUMBER) {
				item = new MenuItemNumber(it);
			} else {
				item = new MenuItem(it);
			}

			data->addItem(item);
		} else {
			data->addItem(nullptr);
		}
	}
}

Menu::~Menu()
{
	exit();
}

void Menu::exit()
{
	if (data) {
		data->clear();
		delete data;
		data = nullptr;
	}

	if (style) {
		delete style;
		style = nullptr;
	}
}

void Menu::draw()
{
	style->draw(data);
}

bool Menu::update()
{
	data->update();
	switch (data->whichSelected()) {
		case ID::QUIT:
			return true;
		break;

		case ID::PLAYMENU:
			createMenu(playMenuItems);
		break;

		case ID::BACK:
			createMenu(mainMenuItems);
		break;

		default:
		break;
	} 

	return false;
}