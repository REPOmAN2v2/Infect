/*
 * https://github.com/REPOmAN2v2/Infect
 * 
 * This header provides a couple functions for the main game loop. getMoves()
 * moves the characters while getActions executes their action, both according
 * to a set of rules. 
 */

#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "include.h"

void getMoves(Board * const * const);
void getActions(Board * const * const );

#endif