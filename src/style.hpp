#pragma once

#include "window.hpp"

class Style
{
public:
	Style(int h, int w);
	virtual ~Style();
	virtual void draw(){};
	Window *main;
};