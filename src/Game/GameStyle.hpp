#pragma once

#include <engine/style.hpp>
#include <engine/window.hpp>
#include <vector>

#include "GameData.hpp"

class GameData;
class Window;

class GameStyle : public Style
{
public:
	GameStyle(int h, int w);
	~GameStyle();
	void draw(GameData *data);
	void resize(int h, int w);
	void create() override;
	Window *game, *info;
private:
	std::vector<Window **> windows; // purely for convenience
	void clearScreen();
	void destroy() override;
	void clear();
	void refresh();
	void setBorders();
};
