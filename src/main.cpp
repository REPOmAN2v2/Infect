#include "ncurses.hpp"
#include "Menu/Menu.hpp"

int main()
{
	Ncurses::init();
	Menu menu;
	menu.createMainMenu();
	menu.draw();
	while (getch() != 'n');
	Ncurses::exit();
}