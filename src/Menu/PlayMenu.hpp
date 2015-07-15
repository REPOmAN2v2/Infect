#pragma once

#include <engine/Menu/Menu.hpp>

class PlayMenu : public Menu {
public:
	PlayMenu(Manager &manager);
	void update() override;
	void load() override;
private:
	void saveSettings() override;
};