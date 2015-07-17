#include "GameStyle.hpp"
#include <engine/Config/Globals.hpp>
#include <algorithm>

GameStyle::GameStyle(int h, int w):
	Style(h, w),
	game(nullptr),
	info(nullptr)
{
	create();
}

void GameStyle::create()
{
	int w, x;
	Style::create();
	destroy();

	w = 2*main->getW()/3 - 2;

	game = new Window(main, -1, w, 1, 1);

	x = w + 4;
	w = main->getW() / 3 - 2;

	info = new Window(main, -1, w, 1, x);
	info->setTitle(std::string("Score"));

	// windows should be pushed from the background to the foreground
	// otherwise expect the unexpected
	windows.push_back(&main);
	windows.push_back(&game);
	windows.push_back(&info);
}

GameStyle::~GameStyle()
{
	destroy();
}

void GameStyle::destroy()
{
	if (game) {
		delete game;
		game = nullptr;
	}

	if (info) {
		delete info;
		info = nullptr;
	}
}

void GameStyle::draw(GameData *data)
{
	clear();
	//game->draw(stuff)
	refresh();
}

void GameStyle::resize(int h, int w)
{
	_h = h;
	_w = w;
	clearScreen();
	create();
}

// TODO: check if that works instead of calling Ncurses functions clear() and refresh() (because it's not anymore)
void GameStyle::clearScreen()
{
	clear();
	refresh();
}

void GameStyle::clear()
{
	std::for_each(windows.begin(), windows.end(), [this](Window** &w){(**w).clear();});
}

void GameStyle::refresh()
{
	std::for_each(windows.rbegin(), windows.rend(), [this](Window** &w){(**w).refresh();});
}

void GameStyle::setBorders()
{
	std::for_each(windows.begin(), windows.end(), [this](Window** &w){(**w).toggleBorders();});
}