#include "ncurses.hpp"
#include "Menu/Menu.hpp"

int main(int argc, char **argv)
{
	Ncurses::init();
	Menu menu;
	menu.createMainMenu();
	menu.draw();
	while (getch() != 'n');
	Ncurses::exit();
}