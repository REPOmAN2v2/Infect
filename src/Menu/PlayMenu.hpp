#pragma once

#include <engine/Menu/Menu.hpp>

class Manager;

class PlayMenu : public Menu {
public:
	explicit PlayMenu(Manager &manager);
	void update() override;
	void load() override;
private:
	void saveSettings() override;
};
