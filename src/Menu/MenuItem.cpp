#include "MenuItem.hpp"

MenuItem::MenuItem(std::string label, int id):label(label),id(id)
{}

void MenuItem::draw(Window *window, bool cur, int w, int y, int x)
{
	window->print(label.substr(0, w), y, x, cur ? COLOR_GREEN : -1, -1);
}