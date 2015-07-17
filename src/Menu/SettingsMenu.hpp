#pragma once

#include <engine/Menu/Menu.hpp>
#include "MenuItemTemplates.hpp"

class SettingsMenu : public Menu {
public:
	SettingsMenu(Manager &manager);
	void update() override;
	void load() override;
private:
	void saveSettings() override;
};