#include "PlayMenu.hpp"
#include "MainMenu.hpp"

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
	item = new MenuItem("Launch game", ID::PLAY, Type::SIMPLE);
	data->addItem(item);
	item = new MenuItem("Back", ID::BACK, Type::SIMPLE);
	data->addItem(item);
	data->addItem(nullptr);
	item = new MenuItemNumber("Doctors", ID::DOCTORS, Type::NUMBER, 0, 100, 5);
	data->addItem(item);
	item = new MenuItemNumber("Infected", ID::INFECTED, Type::NUMBER, 0, 100, 8);
	data->addItem(item);
	item = new MenuItemNumber("Nurses", ID::NURSES, Type::NUMBER, 0, 100, 6);
	data->addItem(item);
	item = new MenuItemNumber("Soldiers", ID::SOLDIERS, Type::NUMBER, 0, 100, 10);
	data->addItem(item);
	item = new MenuItemNumber("Lumber", ID::LUMBER, Type::NUMBER, 0, 1000, 50);
	data->addItem(item);
	data->addItem(nullptr);
	item = new MenuItemList("Sim speed", ID::SPEED, Type::LIST, simSpeed, "Normal");
	data->addItem(item);
	item = new MenuItemToggle("Step", ID::STEP, Type::TOGGLE, false);
	data->addItem(item);
	item = new MenuItem("Reset", ID::RESET, Type::SIMPLE);
	data->addItem(item);
}

void PlayMenu::update()
{
	data->update();
	static State *state = nullptr;

	switch (data->whichSelected()) {
		case ID::PLAY:
			/* State *game = new Game();
			manager.change(game);*/
		break;

		case ID::BACK:
			saveSettings();
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
	// empty for now
}