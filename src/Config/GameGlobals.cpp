#include <engine/Config/config.hpp>

#include "GameGlobals.hpp"

bool GameGlobals::Settings::step = false;
std::string GameGlobals::Settings::speed = "Normal";
int GameGlobals::Constants::doctors = 5;
int GameGlobals::Constants::nurses = 6;
int GameGlobals::Constants::infected = 8;
int GameGlobals::Constants::soldiers = 10;
int GameGlobals::Constants::lumber = 100;

static const char *file = "game.cfg";
static ConfigIO IO;

void GameGlobals::load()
{
	Config *cfg = IO.open(file);
	if (!cfg) return;

	IO.lookup<bool>(cfg, GameGlobals::Settings::step, "settings.settings.step");
	IO.lookup<std::string>(cfg, GameGlobals::Settings::speed, "settings.settings.speed");
	IO.lookup<int>(cfg, GameGlobals::Constants::doctors, "settings.constants.doctors");
	IO.lookup<int>(cfg, GameGlobals::Constants::nurses, "settings.constants.nurses");
	IO.lookup<int>(cfg, GameGlobals::Constants::infected, "settings.constants.infected");
	IO.lookup<int>(cfg, GameGlobals::Constants::soldiers, "settings.constants.soldiers");
	IO.lookup<int>(cfg, GameGlobals::Constants::lumber, "settings.constants.lumber");

	delete cfg;
}

static void writeSettingsMenu(Config *cfg)
{
	// NOTE: Setting& operator=(const Setting&) is private, so we need a new Setting for each value afaik
	Setting &root = cfg->getRoot();
	Setting &settings = IO.addSetting(root, "settings", Setting::TypeGroup);
	{
		Setting &settings2 = IO.addSetting(settings, "settings", Setting::TypeGroup);
		Setting &step = IO.addSetting(settings2, "step", Setting::TypeBoolean);
		Setting &speed = IO.addSetting(settings2, "speed", Setting::TypeString);

		step = GameGlobals::Settings::step;
		speed = GameGlobals::Settings::speed;
	}

	{	Setting &constants = IO.addSetting(settings, "constants", Setting::TypeGroup);
		Setting &doctors = IO.addSetting(constants, "doctors", Setting::TypeInt);
		doctors = GameGlobals::Constants::doctors;
		Setting &nurses = IO.addSetting(constants, "nurses", Setting::TypeInt);
		nurses = GameGlobals::Constants::nurses;
		Setting &infected = IO.addSetting(constants, "infected", Setting::TypeInt);
		infected = GameGlobals::Constants::infected;
		Setting &soldiers = IO.addSetting(constants, "soldiers", Setting::TypeInt);
		soldiers = GameGlobals::Constants::soldiers;
		Setting &lumber = IO.addSetting(constants, "lumber", Setting::TypeInt);
		lumber = GameGlobals::Constants::lumber;
	}
}

void GameGlobals::save()
{
	Config *cfg = IO.open(file);
	if (!cfg) return;
	writeSettingsMenu(cfg);

	IO.write(cfg, file);

	delete cfg;
}
