#pragma once

#include "engine/State/State.hpp"

class GameData;
class GameStyle;
class Manager;

class GameState : public State {
public:
	explicit GameState(Manager &manager);
	void draw() override;
	void update() override;
	void exit() override;
	void load() override;
private:
	void resize();
	void spawnHelp();
	void spawnQuit();
	GameStyle *style;
	GameData *data;
};
