#include "MenuStyle.hpp"

MenuStyle::MenuStyle(int h, int w):
	Style(h, w),
	menu(nullptr)
{
	int hh = main->getH() - 11;
	int ww = main->getW() / 3;
	int y = 11;
	int x = main->getW() / 3;

	menu = new Window(main, hh, ww, y, x);
}

MenuStyle::~MenuStyle()
{
	if (menu) {
		delete menu;
	}
}

void MenuStyle::draw(MenuData *data)
{
	main->clear();
	menu->clear();
	data->draw(menu);
	menu->setBorders();
	main->setBorders();
	menu->refresh();
	main->refresh();
}