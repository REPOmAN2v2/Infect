#pragma once

#include <engine/Menu/Menu.hpp>
#include "MenuItemTemplates.hpp"

class PlayMenu : public Menu {
public:
	PlayMenu(Manager &manager);
	void update() override;
	void load() override;
private:
	void saveSettings() override;
};