#ifndef MENU_H_
#define MENU_H_

#include "include.h"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

void displayMenu();
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void func(char *name);

#endif