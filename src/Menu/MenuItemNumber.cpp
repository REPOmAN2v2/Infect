#include "MenuItemNumber.hpp"

MenuItemNumber::MenuItemNumber(MenuItemTemplate &tem):
	MenuItem(tem),
	min(tem.min),
	max(tem.max),
	start(tem.start),
	current(tem.start)
{}

void MenuItemNumber::draw(Window *window, bool cur, int w, int y, int x)
{
	std::string num = std::to_string(current);

	window->print(label.substr(0, w - num.size()), y, x, cur ? COLOR_GREEN : -1, -1);
	window->print(num, y, w - num.size(), cur ? COLOR_GREEN : -1, -1);
}

void MenuItemNumber::update(int key)
{
	if (key == KEY_LEFT) {
		if ((current - 1) >= min) --current; 
	} else if (key == KEY_RIGHT) {
		if ((current + 1) <= max) ++current;
	}
}