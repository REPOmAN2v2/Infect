#pragma once

#include <engine/Menu/Menu.hpp>

#include "MenuItemTemplates.hpp"

class Manager;

class MainMenu : public Menu {
public:
	explicit MainMenu(Manager &manager);
	void update() override;
	void load() override;
private:
	void saveSettings() override {};
	void spawnHelp();
};
