#include <string.h>
#include "gamemenu.h"

char *choices[] = {
                        "Game width: ",
                        "Game height: ",
						"Doctors: ",
						"Infected: ",
						"Nurses: ",
                        "Soldiers: ",
                        "Wood: ",
                        "Speed: ",
                        "Debug: ",
                        "Recalculate units",
                        "Play",
                        (char *)NULL
                  };

char *speeds[] = {
	"Slow",
	"Fast",
	"Fastest",
	(char *)NULL
};

char *debug[] = {
	"Yes",
	"No",
	(char *)NULL
};

void displayMenu()
{	
	ITEM **my_items = NULL;
	MENU *my_menu = NULL;
	WINDOW *my_menu_win = NULL;
    int n_choices = 0;

	List *listSpeeds = createList(speeds);
	listSpeeds = listSpeeds->previous; // Default value is Faster
	List *listDebug = createList(debug);
	listDebug = listDebug->next; // Default value is No

	int counters[7];
	counters[XC] = 79;
	counters[YC] = 20;
	updateUnits(counters);

	/* Initialize items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	fillItems(my_items, counters, listSpeeds, listDebug);

	/* Initialize curses */	
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

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

	/* Set foreground and background of the menu */
	set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
	set_menu_back(my_menu, COLOR_PAIR(2));
	set_menu_grey(my_menu, COLOR_PAIR(3));

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	mvprintw(LINES - 3, 0, "Up and down arrow keys to navigate");
	mvprintw(LINES - 2, 0, "k and j to increment and decrement");
	mvprintw(LINES - 1, 0, "Enter to select, q to quit");
	refresh();

	if (eventLoop(my_menu, my_menu_win, my_items, listSpeeds, listDebug, counters) == PLAY) {
		updateGameConstants(counters, listSpeeds, listDebug);
		quitMenu(my_menu, my_items, listSpeeds, listDebug);
	} else {
		quitMenu(my_menu, my_items, listSpeeds, listDebug);
		exit(EXIT_SUCCESS);
	}
}

int eventLoop(MENU *my_menu, WINDOW *my_menu_win, ITEM **my_items, List *listSpeeds, List *listDebug, int counters[])
{
	int c = 0;
	ITEM *cur_item = NULL;

	while((c = getch()) != 'q')
	{       
		switch(c) {
			case 'j':
			{
				cur_item = current_item(my_menu);
				if (item_index(cur_item) < 9) {
					toggleValue(cur_item, DOWN, &listSpeeds, &listDebug, counters);
				}
				//	This updates the item description on the screen
				// 	Not sure how to do it otherwise
				menu_driver(my_menu, REQ_DOWN_ITEM);
				menu_driver(my_menu, REQ_UP_ITEM);
			}
				break;

			case 'k':
			{	
				cur_item = current_item(my_menu);
				if (item_index(cur_item) < 9) {
					toggleValue(cur_item, UP, &listSpeeds, &listDebug, counters);
				}
				menu_driver(my_menu, REQ_DOWN_ITEM);
				menu_driver(my_menu, REQ_UP_ITEM);
			}
				break;

			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;

			case ENTER:
			{
				int index = item_index(current_item(my_menu));
				if (index == 9) {
					updateUnitsDisplay(my_items, counters);
					//	Iterate over every menu item to update their displayed value
					for (size_t i = 0; i < 10; i++) {
						menu_driver(my_menu, REQ_UP_ITEM);
					}
				} else if (index == 10) {
					return PLAY;
				}
			}
		} // end switch

		wrefresh(my_menu_win);
	} // end while

	return QUIT;
}

void quitMenu(MENU *my_menu, ITEM **my_items, List *listSpeeds, List *listDebug)
{
	unpost_menu(my_menu);
	freeList(&listSpeeds);
	freeList(&listDebug);
	int n_choices = ARRAY_SIZE(choices);
	for(size_t i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}

void freeList(List **list)
{
	if (list && *list) {
		List* next = (*list)->next;
		while (next && (next != *list)) {
			List *tmp = next;
			next = next->next;
			free(tmp->value);
			free(tmp);
		}

		free((*list)->value);
		free(*list);
		*list = NULL;
	}

}

void fillItems(ITEM **my_items, int counters[], List *listSpeeds, List *listDebug)
{
	/* 	You should only pass strings from the heap, global memory or
		the string literal pool to new_item(). Why? Because if you 
		look through the source, the item description points to the
		string we passed, so you shouldn't pass strings from the stack.
		Freeing the description before changing it is left to the user.

		This results in some ugly things, not only to convert counters 
		to strings, but also to make sure we can free the strings safely 
		later on. This is the best way to do this I've found so far.
		*/

	for (size_t i = XC; i <= WOODC; i++) {
		char buffer[10];
		sprintf(buffer, "%d", counters[i]);
		my_items[i] = new_item(choices[i], convertToHeapString(buffer));
	}

	my_items[7] = new_item(choices[7], convertToHeapString(listSpeeds->value));
	my_items[8] = new_item(choices[8], convertToHeapString(listDebug->value));
	my_items[9] = new_item(choices[9], NULL);
	my_items[10] = new_item(choices[10], NULL);
}

char * convertToHeapString(char *string)
{
	char *heapString = strdup(string);
	if (heapString == NULL) {
		endwin();
		fprintf(stderr, "Could not convert to heap string.\n");
		exit(EXIT_FAILURE);
	}

	return heapString;
}

void updateUnits(int counters[])
{
	counters[DOCC] = (counters[XC] * counters[YC] * 0.01) + 1;
	counters[INFC] = (counters[XC] * counters[YC] * 0.005) + 1;
	counters[SOLC] = (counters[XC] * counters[YC] * 0.02) + 1;
	counters[NURC] = (counters[XC] * counters[YC] * 0.05) + 1;
	counters[WOODC] = (counters[XC] * counters[YC] * 0.5);
}

void updateUnitsDisplay(ITEM **items, int counters[])
{
	char buffer[10];
	updateUnits(counters);

	for (size_t i = 0; i <= WOODC; i++) {
		sprintf(buffer, "%d", counters[i]);
		set_item_description(items[i], convertToHeapString(buffer));
	}
}

void updateGameConstants(int counters[], List *listSpeeds, List *listDebug)
{
	countDoc = counters[DOCC];
	countInf = counters[INFC];
	countNur = counters[NURC];
	countSol = counters[SOLC];
	countWood = counters[WOODC];
	
	if (strcmp(listSpeeds->value, "Slow") == 0) {
		refreshRate = 10;
	} else if (strcmp(listSpeeds->value, "Fast") == 0) {
		refreshRate = 5;
	} else {
		refreshRate = 1;
	}

	stepthrough = strcmp(listDebug->value, "Yes") == 0 ? 1 : 0;
}

void set_item_description (ITEM *item, const char *description)
{   
	if (item->description.str != NULL) {
		free((void*)(item->description).str);
	}

	item->description.length = strlen(description);
	item->description.str = description;
}

List * createList(char *strings[])
{
	List *node = malloc(sizeof(List));
	List *root;

	if (node == NULL) {
		endwin();
		fprintf(stderr, "Couldn't allocate memory\n");
		exit(EXIT_FAILURE);
	}

	node->value = strdup(strings[0]);
	node->next = node->previous = NULL;

	root = node;

	for (size_t i = 1; strings[i] != NULL; i++) {
		List *new = malloc(sizeof(List));
		if (new == NULL) {
			endwin();
			fprintf(stderr, "Couldn't allocate memory\n");
			exit(EXIT_FAILURE);
		}
		new->value = strdup(strings[i]);
		node->next = new;
		new->previous = node;
		new->next = NULL;
		node = new;
	}

	node->next = root;
	root->previous = node;

	return root;
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

void toggleValue(ITEM *item, int direction, List **listSpeeds, List **listDebug, int counters[])
{
	int index = item_index(item);

	if (index == 7) {
		*listSpeeds = direction == UP ? (*listSpeeds)->next : (*listSpeeds)->previous;
		set_item_description(item, convertToHeapString((*listSpeeds)->value));
	} else if (index == 8) {
		*listDebug = direction == UP ? (*listDebug)->next : (*listDebug)->previous;
		set_item_description(item, convertToHeapString((*listDebug)->value));
	} else {
		updateNumericValue(item, direction, counters, index);
	}
}

void updateNumericValue(ITEM *item, int direction, int counters[], int index)
{
	char buffer[10];

	if (direction == UP) {
		sprintf(buffer, "%d", ++counters[index]);
		set_item_description(item, convertToHeapString(buffer));
	} else {
		if (counters[index] == 0) {
			return;
		} else {
			sprintf(buffer, "%d", --counters[index]);
			set_item_description(item, convertToHeapString(buffer));
		}
	}
}