#pragma once

enum class Action {QUIT, HELP, NONE};

class Window;

class GameData 
{
public:
	GameData(int h, int w, int y, int x);
	void update();
	void draw(Window *game);
	Action action;
private:
	int _h, _w, _y, _x;
};