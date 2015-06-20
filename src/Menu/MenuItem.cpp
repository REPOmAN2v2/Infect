#include "MenuItem.hpp"

MenuItem::MenuItem(MenuItemTemplate &tem):
	id(tem.id),
	type(tem.type),
	label(tem.name)
{}

void MenuItem::draw(Window *window, bool cur, int w, int y, int x)
{
	window->print(label.substr(0, w), y, x, cur ? COLOR_GREEN : -1, -1);
}