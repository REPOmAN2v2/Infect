/*
 * https://github.com/REPOmAN2v2/Infect
 * 
 * This header provides functions to initialise the game variables and 
 * generate the game board. parseArgs() is called when the program is launched
 * with command line arguments, bypassing the menu. initDefault() initialises 
 * the default board using the game variables.
 */

#ifndef GENERATION_H_
#define GENERATION_H_

#include "include.h"

Board ** parseArgs(const int argc, const char * const * const argv);
Board **initDefault();

#endif