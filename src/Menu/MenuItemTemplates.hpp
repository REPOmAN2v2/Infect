#pragma once

#include <vector>
#include <string>

// enum classes are nice but we want the implicit conversion to interact with the engine easily
struct ID {
	enum : unsigned int {
		NONE = 0,
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
		RESET,
		//specific to the settings menu
		WIDTH,
		HEIGHT
	};
};
