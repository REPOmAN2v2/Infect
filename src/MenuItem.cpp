#include "MenuItem.hpp"

MenuItem::MenuItem(std::string label, int id):label(label),id(id)
{}

void MenuItem::draw(Window *window, int w, int y, int x)
{
	window->print(label.substr(0, w), y, x);
}