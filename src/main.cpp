#include <Menu/MainMenu.hpp>
#include <engine/Config/Globals.hpp>
#include <engine/State/Manager.hpp>
#include <engine/ncurses.hpp>

#include "Config/GameGlobals.hpp"

class State;

int main()
{
	Globals::load();
	GameGlobals::load();
	Ncurses::init();

	Manager manager;
	State *state = new MainMenu(manager);

	manager.init(state);
	manager.loop();
	
	Ncurses::exit();
	//Globals::save();
	//GameGlobals::save();
}
