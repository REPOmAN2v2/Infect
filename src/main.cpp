#include <engine/ncurses.hpp>
#include <engine/State/Manager.hpp>
#include <Menu/MainMenu.hpp>
#include <engine/Config/Globals.hpp>

int main()
{
	Globals::load();	
	Ncurses::init();

	Manager manager;
	State *state = new MainMenu(manager);

	manager.init(state);
	manager.loop();
	
	Ncurses::exit();
	Globals::save();
}