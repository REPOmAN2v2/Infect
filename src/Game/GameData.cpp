#include "GameData.hpp"
#include <engine/ncurses.hpp>

GameData::GameData(int h, int w, int y, int x):_h(h),_w(w),_y(y),_x(x)
{}

void GameData::update()
{
	int key = Ncurses::getKey(0);
	action = Action::NONE;

	switch (key) {
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