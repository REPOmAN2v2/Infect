#include "MenuStyle.hpp"

MenuStyle::MenuStyle(int h, int w):
	Style(h, w),
	menu(nullptr)
{
	menu = new Window(main,
					  main->getH() - 4,
					  main->getW()/2,
					  main->getH(),
					  main->getW()/4);
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
	menu->refresh();
	main->refresh();
}