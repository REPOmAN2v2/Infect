#pragma once

enum class Type {
	NONE,
	SIMPLE,
	NUMBER,
	TEXT,
	TOGGLE
}; 

enum ID {
	ZERO,
	PLAY,
	//specific to the main menu
	SETTINGS, 
	HELP, 
	QUIT,
	TEST0,
	TEST1,
	TEST2,
	//specific to the play menu
	DOCTORS,
	INFECTED,
	NURSES,
	SOLDIERS,
	LUMBER,
	SPEED,
	STEP,
	RESET
};

struct MenuItemTemplate {
	const char *name;
	enum ID id;
	enum Type type;
};
