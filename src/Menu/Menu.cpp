#include "Menu.hpp"
#include <vector>

static std::vector<MenuItemTemplate> mainMenuItems = {
	{.name = "Play", .id = PLAY, .type = Type::SIMPLE},
	{.name = "Settings", .id = SETTINGS, .type = Type::SIMPLE},
	{.name = "Help", .id = HELP, .type = Type::SIMPLE},
	{.name = "Quit", .id = QUIT, .type = Type::SIMPLE},
	{.name = nullptr, .id = ZERO, .type = Type::NONE},
	{.name = "Test0", .id = TEST0, .type = Type::SIMPLE},
	{.name = "Test1", .id = TEST1, .type = Type::SIMPLE},
	{.name = "Test2", .id = TEST2, .type = Type::SIMPLE}
};

static std::vector<MenuItemTemplate> playMenuItems = {
	{.name = "Launch game", .id = PLAY, .type = Type::SIMPLE},
	{.name = nullptr, .id = ZERO, .type = Type::NONE},
	{.name = "Doctors", .id = DOCTORS, .type = Type::NUMBER},
	{.name = "Infected", .id = INFECTED, .type = Type::NUMBER},
	{.name = "Nurses", .id = NURSES, .type = Type::NUMBER},
	{.name = "Soldiers", .id = SOLDIERS, .type = Type::NUMBER},
	{.name = "Lumber", .id = LUMBER, .type = Type::NUMBER},
	{.name = nullptr, .id = ZERO, .type = Type::NONE},
	{.name = "Sim speed", .id = SPEED, .type = Type::TEXT},
	{.name = "Step", .id = STEP, .type = Type::TOGGLE},
	{.name = "Reset", .id = RESET, .type = Type::SIMPLE}
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