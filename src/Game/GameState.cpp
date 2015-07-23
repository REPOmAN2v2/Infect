#include <engine/Dialog/Dialog.hpp> // for Dialog::message(2)
#include <string>

#include "../Menu/MainMenu.hpp"
#include "Game/GameData.hpp"
#include "Game/GameStyle.hpp"
#include "GameState.hpp"
#include "engine/Menu/../Config/Globals.hpp"
#include "engine/Menu/Items/../../window.hpp" // for Window class (style->game->get<X>(0))
#include "engine/State/Manager.hpp"

GameState::GameState(Manager &manager):State(manager),style(nullptr),data(nullptr)
{
	if (!style) {
		style = new GameStyle(Globals::Settings::height, Globals::Settings::width);
	}
}

void GameState::load()
{
	if (data) {
		delete data;
		data = nullptr;
	}

	data = new GameData(style->game->getH() - 2,
						style->game->getW() - 2,
						1,1);
}

void GameState::exit()
{
	if (style) {
		delete style;
		style = nullptr;
	}

	if (data) {
		delete data;
		data = nullptr;
	}
}

void GameState::draw()
{
	style->draw(data);
}

void GameState::resize()
{
	style->resize(Globals::Settings::height, Globals::Settings::width);
}

void GameState::update()
{
	data->update();

	static State *state = nullptr;

	switch (data->action) {
		case Action::QUIT:
			state = new MainMenu(manager);
			manager.change(state);
			state = nullptr;
		break;

		case Action::HELP:
			spawnHelp();
		break;

		default:
		break;
	}
}

void GameState::spawnHelp()
{
	std::string message = "Cursed engine v" VERSION "\n"
						  "Compiled " DATE "\n"
						  "https://github.com/REPOmAN2v2/Cursed-engine";
	Dialog::message(message, std::string("Help"));
}

void GameState::spawnQuit()
{
	std::string message = "Are you sure you want to quit?";
	Dialog::message(message, std::string("Quit"));
}
