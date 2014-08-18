#include <menu.h>
#include <string.h>
#include "gamemenu.h"

#define CTRLD 	4

char *choices[] = {
                        "Speed: ",
                        "Debug: ",
                        "Game width: ",
                        "Game height: ",
						"Doctors: ",
						"Infected: ",
						"Nurses: ",
                        "Soldiers: ",
                        "Wood: ",
                        "Exit",
                        (char *)NULL,
                  };

void displayMenu()
{	
	ITEM **my_items;
	int c;				
	MENU *my_menu;
    int n_choices;
	ITEM *cur_item;
	WINDOW *my_menu_win;
	
	/* Initialize curses */	
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

	/* Initialize items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	my_items[0] = new_item(choices[0], "Fast");
	my_items[1] = new_item(choices[1], "No");
	my_items[2] = new_item(choices[2], "79");
	my_items[3] = new_item(choices[3], "20");
	my_items[4] = new_item(choices[4], "Default");
	my_items[5] = new_item(choices[5], "Default");
	my_items[6] = new_item(choices[6], "Default");
	my_items[7] = new_item(choices[7], "Default");
	my_items[8] = new_item(choices[8], "Default");
	my_items[9] = new_item(choices[9], NULL);
	for(size_t i = 0; i < n_choices; ++i) {
		/* Set the user pointer */
		set_item_userptr(my_items[i], func);
	}

	/* Get offsets */
	int offsetx = (COLS - 40) / 2;
    int offsety = 0;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
	my_menu_win = newwin(15, 40, offsety, offsetx);
	keypad(my_menu_win, TRUE);

	/* Set main window and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 12, 38, 3, 1));
	set_menu_format(my_menu, 16, 1);

	/* Set menu mark */
	set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Infect", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
	set_menu_back(my_menu, COLOR_PAIR(2));
	set_menu_grey(my_menu, COLOR_PAIR(3));

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up/k and Down/j keys to navigate (q to Exit)");
	refresh();

	while((c = getch()) != 'q')
	{       
		switch(c) {
			case 'j':	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case 'k':
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10: /* Enter */
			{	
				void (*p)(char *);

				cur_item = current_item(my_menu);
				p = item_userptr(cur_item);
				p((char *)item_name(cur_item));
				pos_menu_cursor(my_menu);
				break;
			}
			break;
		}
		wrefresh(my_menu_win);
	}	
	unpost_menu(my_menu);
	for(size_t i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
	exit(EXIT_SUCCESS);
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
	int length, x, y;
	float temp;

	if (win == NULL) {
		win = stdscr;
	}

	getyx(win, y, x);
	if (startx != 0) {
		x = startx;
	}

	if (starty != 0) {
		y = starty;
	}

	if (width == 0) {
		width = 80;
	}

	length = strlen(string);
	temp = (width - length) / 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}


void func(char *name)
{	
	move(20, 0);
	clrtoeol();
	mvprintw(20, 0, "Item selected is : %s", name);
}