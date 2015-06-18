#include "MenuData.hpp"

MenuData::MenuData(int h, int w, int y, int x):
	_h(h),
	_w(w),
	_y(y),
	_x(x),
	current(nullptr),
	index(0)
{}

MenuData::~MenuData()
{
	for (size_t i = 0; i < items.size(); ++i) {
		if (items[i]) {
			delete(items[i]);
			items[i] = nullptr;
		}
	}

	items.clear();
}

void MenuData::addItem(MenuItem *item)
{
	items.push_back(item);

	if (items.size() == 1) {
		current = items.back();
	}

	for (size_t i = 0; i < items.size(); ++i) {
		if (items[i]) {
			current = items[i];
			index = i;
			break;
		}
	}
}

void MenuData::removeItem(int id)
{
	std::vector<MenuItem*>::iterator it = items.begin();
	for (; it != items.end(); ++it) {
		if (!(*it)) {
			continue;
		}

		if ((*it)->id == id) {
			if (*it == current) {
				nextItem();
				index--;
			}

			items.erase(it);
			return;
		}
	}
}

void MenuData::nextItem()
{
	if (items.size() == 0) {
		return;
	} else if (items.size() == 1) {
		current = items.back();
		index = 0;
		return;
	} else if (current == items.back()) {
		// check the menu does not contain any empty items
		if (index == (items.size() - 1)) {
			firstItem();
			return;
		}
	}

	++index;
	current = items[index];

	if (!current) {
		nextItem();
	}
}

void MenuData::prevItem()
{
	if (items.size() == 0) {
		return;
	} else if (items.size() == 1) {
		current = items.front();
		index = 0;
		return;
	} else if (current == items.front()) {
		if (index == 0) {
			lastItem();
			return;
		}
	}

	++index;
	current = items[index];

	if (!current) {
		prevItem();
	}
}

void MenuData::firstItem()
{
	if (items.size() == 0) {
		return;
	} else if (items.size() == 1) {
		current = items.front();
		index = 0;
		return;
	}

	current = items.front();
	index = 0;

	if (!current) {
		nextItem();
	}
}

void MenuData::lastItem()
{
	if (items.size() == 0) {
		return;
	} else if (items.size() == 1) {
		current = items.front();
		index = 0;
		return;
	}

	current = items.back();
	index = items.size() - 1;

	if (!current) {
		prevItem();
	}
}

void MenuData::draw(Window *window)
{
	size_t begin = 0;
	size_t end = items.size();

	if (_h < items.size()) {
		if (index <= _h/2) {
			end = _h - 1;
		} else if (index < (items.size() - _h/2) - 1) {
			begin = index - _h/2;
			end = index + _h/2;
		} else {
			begin = items.size() - _h;
		}
	}

	for (size_t i = begin, dy = 0; i < end; ++i, ++dy) {
		if ((i == begin) && (i != 0)) {
			window->print("(more)",
						  _x + _w/2 - 3, // center the text
						  _y + dy);
			continue;
		} else if ((i == end - 1) && (i != items.size() - 1)) {
			window->print("(more)",
						  _x + _w/2 - 3,
						  _y + dy + 1);
			continue;
		} else if (!items[i]) {
			std::string line;
			line.append(_w, '-');
			window->print(line,
						  _x + _w/2 - line.size()/2,
						  _y + dy);
		} else {
			items[i]->draw(window, _w, _y + dy, _x);
		}
	}
}