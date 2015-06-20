#pragma once

enum class Type {
	NONE,
	SIMPLE,
	NUMBER,
	TEXT,
	TOGGLE
}; 

enum class ID {
	NONE,
	BACK,
	//specific to the main menu
	PLAYMENU,
	SETTINGS, 
	HELP, 
	QUIT,
	TEST0,
	TEST1,
	TEST2,
	//specific to the play menu
	PLAY,
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
	int min, max, start;
};
