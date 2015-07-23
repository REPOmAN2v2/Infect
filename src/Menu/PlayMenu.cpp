#include <engine/Dialog/Dialog.hpp>
#include <string>
#include <vector>

#include "../Config/GameGlobals.hpp"
#include "../Game/GameState.hpp"
#include "MainMenu.hpp"
#include "Menu/MenuItemTemplates.hpp"
#include "PlayMenu.hpp"
#include "engine/Menu/../State/Manager.hpp"
#include "engine/Menu/Items/MenuItem.hpp"
#include "engine/Menu/Items/MenuItemList.hpp"
#include "engine/Menu/Items/MenuItemNumber.hpp"
#include "engine/Menu/Items/MenuItemToggle.hpp"
#include "engine/Menu/Menu.hpp"
#include "engine/Menu/MenuData.hpp"
#include "engine/Menu/MenuStyle.hpp"
#include "engine/window.hpp"

class State;

PlayMenu::PlayMenu(Manager &manager):Menu(manager) {};

void PlayMenu::load()
{
	if (data) {
		delete data;
		data = nullptr;
	}

	data = new MenuData(style->menu->getH() - 2,
						style->menu->getW() - 2,
						1,1);

	static const std::vector<std::string> simSpeed = {"Slow", "Normal", "Fast"};

	MenuItem *item;
	item = new MenuItem("Launch game", ID::PLAY, MenuItem::Type::SIMPLE);
	data->addItem(item);
	item = new MenuItem("Back", ID::BACK, MenuItem::Type::SIMPLE);
	data->addItem(item);
	data->addItem(nullptr);
	item = new MenuItemNumber("Doctors", ID::DOCTORS, MenuItem::Type::NUMBER, 0, 100, GameGlobals::Constants::doctors);
	data->addItem(item);
	item = new MenuItemNumber("Infected", ID::INFECTED, MenuItem::Type::NUMBER, 0, 100, GameGlobals::Constants::infected);
	data->addItem(item);
	item = new MenuItemNumber("Nurses", ID::NURSES, MenuItem::Type::NUMBER, 0, 100, GameGlobals::Constants::nurses);
	data->addItem(item);
	item = new MenuItemNumber("Soldiers", ID::SOLDIERS, MenuItem::Type::NUMBER, 0, 100, GameGlobals::Constants::soldiers);
	data->addItem(item);
	item = new MenuItemNumber("Lumber", ID::LUMBER, MenuItem::Type::NUMBER, 0, 1000, GameGlobals::Constants::lumber);
	data->addItem(item);
	data->addItem(nullptr);
	item = new MenuItemList("Sim speed", ID::SPEED, MenuItem::Type::LIST, simSpeed, GameGlobals::Settings::speed);
	data->addItem(item);
	item = new MenuItemToggle("Step", ID::STEP, MenuItem::Type::TOGGLE, GameGlobals::Settings::step);
	data->addItem(item);
	item = new MenuItem("Reset", ID::RESET, MenuItem::Type::SIMPLE);
	data->addItem(item);
}

void PlayMenu::update()
{
	data->update();
	static State *state = nullptr;

	switch (data->whichSelected()) {
		case ID::PLAY:
			state = new GameState(manager);
			manager.change(state);
		break;

		case ID::BACK:
			if (Dialog::prompt(std::string("Do you want to save?"), std::string("Save game settings"))) {
				saveSettings();
			}

			state = new MainMenu(manager);
			changeMenu(state);
			state = nullptr;
		break;

		case ID::RESET:
		// overkill but easier that way right now
		// TODO: iterate over items and reset them one by one
		// probably faster and they already store their original value
			load(); 
		break;

		default:
		break;
	}
}

void PlayMenu::saveSettings()
{
	GameGlobals::Constants::doctors = data->get<int>(ID::DOCTORS);
	GameGlobals::Constants::nurses = data->get<int>(ID::NURSES);
	GameGlobals::Constants::infected = data->get<int>(ID::INFECTED);
	GameGlobals::Constants::soldiers = data->get<int>(ID::SOLDIERS);
	GameGlobals::Constants::lumber = data->get<int>(ID::LUMBER);
	GameGlobals::Settings::speed = data->get<std::string>(ID::SPEED);
	GameGlobals::Settings::step = data->get<bool>(ID::STEP);
}
