#pragma once

#include <string>

namespace GameGlobals
{
	void load();
	void save();

	namespace Settings
	{
		extern bool step;
		extern std::string speed;
	}

	namespace Constants
	{
		extern int doctors;
		extern int nurses;
		extern int infected;
		extern int soldiers;
		extern int lumber;
	}
}