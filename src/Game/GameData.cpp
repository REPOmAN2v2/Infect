#include "GameData.hpp"
#include "engine/Keys.hpp"
#include "engine/ncurses.hpp"
#include "engine/window.hpp"
#include "engine/Config/Globals.hpp"

GameData::GameData(int h, int w, int y, int x):action(Action::NONE),_h(h),_w(w),_y(y),_x(x)
{}

// NOTE: tmp variable for testing purposes
static Key tmp(Key::UNKNOWN);

void GameData::update()
{
	tmp = Ncurses::getKey(-1);
	action = Action::NONE;

	switch (tmp.val) {
		case 'h':
			action = Action::HELP;
		break;

		case 'q':
			action = Action::QUIT;
		break;

		default:
		break;
	}
}

void GameData::draw(Window *game)
{
	game->print(toStr(tmp), 1, 1, Globals::text["normal"]);
}