#include "Menu.hpp"

struct MainMenu {
	const char *name;
	enum ID {
		ZERO,
		PLAY,
		SETTINGS, 
		HELP, 
		QUIT,
		TEST0,
		TEST1,
		TEST2
	} id;
	enum Type {
		NONE,
		SIMPLE
	} type;
};

static struct MainMenu mainMenuItems[] = {
	{.name = "Play", .id = MainMenu::PLAY, .type = MainMenu::SIMPLE},
	{.name = "Settings", .id = MainMenu::SETTINGS, .type = MainMenu::SIMPLE},
	{.name = "Help", .id = MainMenu::HELP, .type = MainMenu::SIMPLE},
	{.name = "Quit", .id = MainMenu::QUIT, .type = MainMenu::SIMPLE},
	{.name = nullptr, .id = MainMenu::ZERO, .type = MainMenu::NONE},
	{.name = "Test0", .id = MainMenu::TEST0, .type = MainMenu::SIMPLE},
	{.name = "Test1", .id = MainMenu::TEST1, .type = MainMenu::SIMPLE},
	{.name = "Test2", .id = MainMenu::TEST2, .type = MainMenu::SIMPLE}
};

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