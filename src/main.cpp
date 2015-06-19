#include "ncurses.hpp"
#include "Menu/Menu.hpp"

int main()
{
	Ncurses::init();
	Menu menu;
	menu.createMainMenu();
	menu.draw();

	while (Ncurses::getKey(100) != 'n') {
		menu.update();
		menu.draw();
	}

	Ncurses::exit();
}