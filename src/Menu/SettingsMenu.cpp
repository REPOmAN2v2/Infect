#include "SettingsMenu.hpp"
#include "MainMenu.hpp"
#include <engine/Dialog/Dialog.hpp>

SettingsMenu::SettingsMenu(Manager &manager):Menu(manager) {};

void SettingsMenu::load()
{
	if (data) {
		delete data;
		data = nullptr;
	}

	data = new MenuData(style->menu->getH() - 2,
						style->menu->getW() - 2,
						1,1);

	MenuItem *item;
	item = new MenuItemNumber("Width", ID::WIDTH, MenuItem::Type::NUMBER, -1, Globals::Settings::max_width, Globals::Settings::width);
	data->addItem(item);
	item = new MenuItemNumber("Height", ID::HEIGHT, MenuItem::Type::NUMBER, -1, Globals::Settings::max_height, Globals::Settings::height);
	data->addItem(item);
	data->addItem(nullptr);
	item = new MenuItem("Reset", ID::RESET, MenuItem::Type::SIMPLE);
	data->addItem(item);
	item = new MenuItem("Back", ID::BACK, MenuItem::Type::SIMPLE);
	data->addItem(item);
}

void SettingsMenu::update()
{
	data->update();
	static State *state = nullptr;

	switch (data->whichSelected()) {
		case ID::BACK:
			if (Dialog::prompt(std::string("Do you want to save?"), std::string("Save settings"))) {
				saveSettings();
				resize();
			}
			
			state = new MainMenu(manager);
			changeMenu(state);
			state = nullptr;
		break;

		case ID::RESET:
			load();
		break;

		default:
		break;
	}
}

void SettingsMenu::saveSettings()
{
	Globals::Settings::width = data->get<int>(ID::WIDTH);
	Globals::Settings::height = data->get<int>(ID::HEIGHT);
}