#include <engine/Dialog/Dialog.hpp> // for Dialog::message(2)
#include <string>

#include "MainMenu.hpp"
#include "Menu/MenuItemTemplates.hpp" // for ID namespace
#include "PlayMenu.hpp"
#include "SettingsMenu.hpp" 
#include "engine/State/Manager.hpp"
#include "engine/window.hpp" // for Window class (style->menu->get<X>(0))
#include "engine/Menu/Items/MenuItem.hpp" 
#include "engine/Menu/MenuData.hpp"
#include "engine/Menu/MenuStyle.hpp" 

class State;

MainMenu::MainMenu(Manager &manager):Menu(manager) {};

void MainMenu::load()
{
	if (data) {
		delete data;
		data = nullptr;
	}

	data = new MenuData(style->menu->getH() - 2,
						style->menu->getW() - 2,
						1,1);

	MenuItem *item;
	item = new MenuItem("Play", ID::PLAYMENU, MenuItem::Type::SIMPLE);
	data->addItem(item);
	item = new MenuItem("Settings", ID::SETTINGS, MenuItem::Type::SIMPLE);
	data->addItem(item);
	item = new MenuItem("Help", ID::HELP, MenuItem::Type::SIMPLE);
	data->addItem(item);
	item = new MenuItem("Quit", ID::QUIT, MenuItem::Type::SIMPLE);
	data->addItem(item);
}

void MainMenu::update()
{
	data->update();
	static State *state = nullptr;

	switch (data->whichSelected()) {
		case ID::QUIT:
			manager.exit();
		break;

// TODO: this is very wasteful. Find a system (instances?) to only recreate the xxxxMenu,
// not the parent classes all over again. Is it even possible?
		case ID::PLAYMENU: 
			state = new PlayMenu(manager);
			changeMenu(state);
			state = nullptr;
		break;

		case ID::SETTINGS:
			state = new SettingsMenu(manager);
			changeMenu(state);
			state = nullptr;
		break;

		case ID::HELP:
			spawnHelp();
		break;

		default:
		break;
	}
}

void MainMenu::spawnHelp()
{
	std::string message = "Cursed engine v" VERSION "\n"
						  "Compiled " DATE "\n"
						  "https://github.com/REPOmAN2v2/Cursed-engine";
	Dialog::message(message, std::string("Help"));
}
