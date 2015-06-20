#include "Menu.hpp"
#include <vector>

static std::vector<MenuItemTemplate> mainMenuItems = {
	{.name = "Play", .id = ID::PLAYMENU, .type = Type::SIMPLE},
	{.name = "Settings", .id = ID::SETTINGS, .type = Type::SIMPLE},
	{.name = "Help", .id = ID::HELP, .type = Type::SIMPLE},
	{.name = "Quit", .id = ID::QUIT, .type = Type::SIMPLE},
	{.name = nullptr, .id = ID::NONE, .type = Type::NONE},
	{.name = "Test0", .id = ID::TEST0, .type = Type::SIMPLE},
	{.name = "Test1", .id = ID::TEST1, .type = Type::SIMPLE},
	{.name = "Test2", .id = ID::TEST2, .type = Type::SIMPLE}
};

static std::vector<MenuItemTemplate> playMenuItems = {
	{.name = "Launch game", .id = ID::PLAY, .type = Type::SIMPLE},
	{.name = "Back", .id = ID::BACK, .type = Type::SIMPLE},
	{.name = nullptr, .id = ID::NONE, .type = Type::NONE},
	{.name = "Doctors", .id = ID::DOCTORS, .type = Type::NUMBER},
	{.name = "Infected", .id = ID::INFECTED, .type = Type::NUMBER},
	{.name = "Nurses", .id = ID::NURSES, .type = Type::NUMBER},
	{.name = "Soldiers", .id = ID::SOLDIERS, .type = Type::NUMBER},
	{.name = "Lumber", .id = ID::LUMBER, .type = Type::NUMBER},
	{.name = nullptr, .id = ID::NONE, .type = Type::NONE},
	{.name = "Sim speed", .id = ID::SPEED, .type = Type::TEXT},
	{.name = "Step", .id = ID::STEP, .type = Type::TOGGLE},
	{.name = "Reset", .id = ID::RESET, .type = Type::SIMPLE}
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
			MenuItem *item = new MenuItem(it.name, it.id);
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
		delete data;
	}

	if (style) {
		delete style;
	}
}

void Menu::draw()
{
	style->draw(data);
}

bool Menu::update()
{
	data->update();
	// TODO: "optimise" to jump table
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