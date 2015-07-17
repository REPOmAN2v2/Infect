#pragma once

#include <engine/State/State.hpp>
#include <engine/Config/Globals.hpp>
#include "GameStyle.hpp"
#include "GameData.hpp"

class GameState : public State {
public:
	GameState(Manager &manager);
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