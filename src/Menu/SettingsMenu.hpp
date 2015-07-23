#pragma once

#include <engine/Menu/Menu.hpp>

#include "MenuItemTemplates.hpp"

class Manager;

class SettingsMenu : public Menu {
public:
	explicit SettingsMenu(Manager &manager);
	void update() override;
	void load() override;
private:
	void saveSettings() override;
};
