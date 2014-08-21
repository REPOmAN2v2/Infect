#include <string.h>
#include "gamemenu.h"
#include "list.h"
#include "display.h"

/*
 * Constants and defines
 */

#define ENTER 	10

enum {DOWN = -1, UP = 1};
enum {QUIT, PLAY};
enum {XC, YC, DOCC, INFC, NURC, SOLC, WOODC};

/* 
 * Arrays containing the menu item names and descriptions 
 */

static char *choices[] = {     
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

static char *speeds[] = {
	"Slow",
	"Fast",
	"Fastest",
	(char *)NULL
};

static char *debug[] = {
	"Yes",
	"No",
	(char *)NULL
};

/*
 * Internal global variables
 */

static ITEM **my_items = NULL;
static MENU *my_menu = NULL;
static WINDOW *my_menu_win = NULL;

/*
 * Internal function prototypes
 */

static int eventLoop(List **listSpeeds, List **listDebug, int counters[]);
static void quitMenu(List *listSpeeds, List *listDebug);
static void fillItems(int counters[], List *listSpeeds, List *listDebug);
static char * convertToHeapString(char *string);
static void updateUnits(int counters[]);
static void updateUnitsDisplay(int counters[]);
static void updateGameVariables(int counters[], List *listSpeeds, List *listDebug);
static void set_item_description (ITEM *item, const char *description);
static void toggleValue(ITEM *item, int direction, List **listSpeeds, List **listDebug, int counters[]);
static void updateNumericValue(ITEM *item, int direction, int counters[], int index);

void displayMenu()
{	
    int n_choices = 0;

	List *listSpeeds = createCircularLinkedList(speeds);
	listSpeeds = listSpeeds->previous; // Default value is Faster
	List *listDebug = createCircularLinkedList(debug);
	listDebug = listDebug->next; // Default value is No

	int counters[7];
	counters[XC] = 79;
	counters[YC] = 20;
	updateUnits(counters);

	/* Initialize items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	fillItems(counters, listSpeeds, listDebug);

	/* Initialize curses */
	initNcurses();

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
	set_menu_grey(my_menu, COLOR_PAIR(4));

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	mvprintw(LINES - 3, 0, "Up and down arrow keys to navigate");
	mvprintw(LINES - 2, 0, "k and j to increment and decrement");
	mvprintw(LINES - 1, 0, "Enter to select, q to quit");
	refresh();

	if (eventLoop(&listSpeeds, &listDebug, counters) == PLAY) {
		updateGameVariables(counters, listSpeeds, listDebug);
		quitMenu(listSpeeds, listDebug);
	} else {
		quitMenu(listSpeeds, listDebug);
		exit(EXIT_SUCCESS);
	}
}

int eventLoop(List **listSpeeds, List **listDebug, int counters[])
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
					toggleValue(cur_item, DOWN, listSpeeds, listDebug, counters);
				}
				//	This refreshes the item description on the screen
				// 	Not sure how to do it otherwise
				menu_driver(my_menu, REQ_DOWN_ITEM);
				menu_driver(my_menu, REQ_UP_ITEM);
			}
				break;

			case 'k':
			{	
				cur_item = current_item(my_menu);
				if (item_index(cur_item) < 9) {
					toggleValue(cur_item, UP, listSpeeds, listDebug, counters);
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
					updateUnitsDisplay(counters);
					//	Iterate over every menu item to refresh their displayed value
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

void quitMenu(List *listSpeeds, List *listDebug)
{
	unpost_menu(my_menu);
	freeCircularLinkedList(&listSpeeds);
	freeCircularLinkedList(&listDebug);
	int n_choices = ARRAY_SIZE(choices);
	for(size_t i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}

void fillItems(int counters[], List *listSpeeds, List *listDebug)
{
	/* 	
	 *	You should only pass strings from the heap, global memory or
	 *	the string literal pool to new_item(). Why? Because if you 
	 *	look through the source, the item description points to the
	 *	string we passed, so you shouldn't pass strings from the stack.
	 *	Freeing the description before changing it is left to the user.
	 *
	 *	This results in some ugly things, not only to convert counters 
	 *	to strings, but also to make sure we can free the strings safely 
	 *	later on. This is the best way to do this I've found so far.
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
		printError("Could not convert to heap string.\n");
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

void updateUnitsDisplay(int counters[])
{
	char buffer[10];
	updateUnits(counters);

	for (size_t i = 0; i <= WOODC; i++) {
		sprintf(buffer, "%d", counters[i]);
		set_item_description(my_items[i], convertToHeapString(buffer));
	}
}

void updateGameVariables(int counters[], List *listSpeeds, List *listDebug)
{
	Units *units = &gameVar.units;
	Time *times = &gameVar.time;

	gameVar.dim.x = counters[XC];
	gameVar.dim.y = counters[YC];

	units->doctors = counters[DOCC];
	units->infected = counters[INFC];
	units->nurses = counters[NURC];
	units->soldiers = counters[SOLC];
	units->wood = counters[WOODC];
	
	if (strcmp(listSpeeds->value, "Slow") == 0) {
		times->refreshRate = SLOW;
	} else if (strcmp(listSpeeds->value, "Fast") == 0) {
		times->refreshRate = FAST;
	} else {
		times->refreshRate = FASTEST;
	}

	times->steps = strcmp(listDebug->value, "Yes") == 0 ? 1 : 0;
}

void set_item_description (ITEM *item, const char *description)
{   
	if (item->description.str != NULL) {
		free((void*)(item->description).str);
	}

	item->description.length = strlen(description);
	item->description.str = description;
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