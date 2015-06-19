#pragma once

enum class Type {
	NONE,
	SIMPLE,
	NUMBER,
	TEXT,
	TOGGLE
}; 

struct MainMenu {
	const char *name;
	enum ID {
		ZERO,
		PLAY,
		SETTINGS, 
		HELP, 
		QUIT,
		TEST0,
		TEST1,
		TEST2
	} id;
	enum Type type;
};

static struct MainMenu mainMenuItems[] = {
	{.name = "Play", .id = MainMenu::PLAY, .type = Type::SIMPLE},
	{.name = "Settings", .id = MainMenu::SETTINGS, .type = Type::SIMPLE},
	{.name = "Help", .id = MainMenu::HELP, .type = Type::SIMPLE},
	{.name = "Quit", .id = MainMenu::QUIT, .type = Type::SIMPLE},
	{.name = nullptr, .id = MainMenu::ZERO, .type = Type::NONE},
	{.name = "Test0", .id = MainMenu::TEST0, .type = Type::SIMPLE},
	{.name = "Test1", .id = MainMenu::TEST1, .type = Type::SIMPLE},
	{.name = "Test2", .id = MainMenu::TEST2, .type = Type::SIMPLE}
};

struct PlayMenu {
	const char *name;
	enum ID {
		ZERO,
		PLAY,
		DOCTORS,
		INFECTED,
		NURSES,
		SOLDIERS,
		LUMBER,
		SPEED,
		STEP,
		RESET
	} id;
	enum Type type;
};

static struct PlayMenu playMenuItems[] = {
	{.name = "Launch game", .id = PlayMenu::PLAY, .type = Type::SIMPLE},
	{.name = nullptr, .id = PlayMenu::ZERO, .type = Type::NONE},
	{.name = "Doctors", .id = PlayMenu::DOCTORS, .type = Type::NUMBER},
	{.name = "Infected", .id = PlayMenu::INFECTED, .type = Type::NUMBER},
	{.name = "Nurses", .id = PlayMenu::NURSES, .type = Type::NUMBER},
	{.name = "Soldiers", .id = PlayMenu::SOLDIERS, .type = Type::NUMBER},
	{.name = "Lumber", .id = PlayMenu::LUMBER, .type = Type::NUMBER},
	{.name = nullptr, .id = PlayMenu::ZERO, .type = Type::NONE},
	{.name = "Sim speed", .id = PlayMenu::SPEED, .type = Type::TEXT},
	{.name = "Step", .id = PlayMenu::STEP, .type = Type::TOGGLE},
	{.name = "Reset", .id = PlayMenu::RESET, .type = Type::SIMPLE}
};