#include <string.h>
#include "gamemenu.h"

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
	ITEM **my_items;
	int c;				
	MENU *my_menu;
    int n_choices, index = 0;
	ITEM *cur_item;
	WINDOW *my_menu_win;


	List *listSpeeds = createList(speeds);
	listSpeeds = listSpeeds->previous; // Default value is Fast
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

	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up/k and Down/j keys to navigate (q to Exit)");
	refresh();

	while((c = getch()) != 'q')
	{       
		switch(c) {
			case 'j':
			{
				cur_item = current_item(my_menu);
				p toggleDown = item_userptr(cur_item);
				toggleDown(cur_item, DOWN, &listSpeeds, &listDebug);
				// listTest(my_menu_win);
				// menu_driver(my_menu, REQ_TOGGLE_ITEM);
			}
				break;
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case 'k':
			{	cur_item = current_item(my_menu);
				p toggleUp = item_userptr(cur_item);
				toggleUp(cur_item, UP, &listSpeeds, &listDebug);
				// listTest(my_menu_win);
				menu_driver(my_menu, REQ_TOGGLE_ITEM);
			}
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10:  /*Enter*/ 			{	
				/*cur_item = current_item(my_menu);
				//index = item_index(current_item(my_menu));
				p = item_userptr(cur_item);
				p(cur_item, UP);
				//pos_menu_cursor(my_menu);
				menu_driver(my_menu, REQ_TOGGLE_ITEM);*/
				listTest(my_menu_win, listSpeeds);
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

void fillItems(ITEM **my_items, int counters[], List *listSpeeds, List *listDebug)
{
	/* 	You should only pass strings from the heap, global memory or
		the string literal pool to new_item(). Why? Because if you 
		look through the source, the item description points to the
		string we passed, so you shouldn't pass strings in the stack.
		Freeing the description before changing it is left to the user.

		This results in some ugly things, not only to convert counters 
		to strings, but also to make sure we can free the strings safely 
		later on.
		*/

	my_items[0] = new_item(choices[0], convertToHeapString(listSpeeds->value));
	set_item_userptr(my_items[0], toggleValue);

	my_items[1] = new_item(choices[1], convertToHeapString(listDebug->value));
	set_item_userptr(my_items[1], toggleValue);

	for (size_t i = XC; i <= WOODC; i++) {
		char buffer[10];
		sprintf(buffer, "%d", counters[i]);
		my_items[i+2] = new_item(choices[i+2], convertToHeapString(buffer));
		set_item_userptr(my_items[i+2], func);
	}

	my_items[9] = new_item(choices[9], NULL);
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

void updateUnits(int counter[])
{
	counter[DOCC] = (counter[XC] * counter[YC] * 0.01) + 1;
	counter[INFC] = (counter[XC] * counter[YC] * 0.005) + 1;
	counter[SOLC] = (counter[XC] * counter[YC] * 0.02) + 1;
	counter[NURC] = (counter[XC] * counter[YC] * 0.05) + 1;
	counter[WOODC] = (counter[XC] * counter[YC] * 0.5);
}

void listTest(WINDOW *win, List *listSpeeds)
{
	move(LINES - 4, 0);
	clrtoeol();
	mvprintw(LINES - 4, 0, "STARTING TEST");
	refresh();
	sleep(1);
	List *test = listSpeeds;
	int i = 0;
	do {
		move(LINES - 4, 0);
		clrtoeol();
		mvprintw(LINES - 4, 0, "%s", test->value);
		test = test->next;
		refresh();
		sleep(1);
	} while (++i < 6);

	move(LINES - 4, 0);
	clrtoeol();
	mvprintw(LINES - 4, 0, "STARTING TEST");
	refresh();
	sleep(1);
	i = 0;
	do {
		move(LINES - 4, 0);
		clrtoeol();
		mvprintw(LINES - 4, 0, "%s", test->value);
		test = test->previous;
		refresh();
		sleep(1);
	} while (++i < 6);

	move(LINES - 4, 0);
	clrtoeol();
	mvprintw(LINES - 4, 0, "DONE");
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
		fprintf(stderr, "Couldn't allocate memory\n");
		exit(EXIT_FAILURE);
	}

	node->value = strdup(strings[0]);
	node->next = node->previous = NULL;

	root = node;
	// fprintf(stdout, "Node: %s, Root: %s\n", node->value, root->value);

	for (size_t i = 1; strings[i] != NULL; i++) {
		List *new = malloc(sizeof(List));
		if (new == NULL) {
			fprintf(stderr, "Couldn't allocate memory\n");
			exit(EXIT_FAILURE);
		}
		new->value = strdup(strings[i]);
		node->next = new;
		new->previous = node;
		new->next = NULL;
		node = new;
		// fprintf(stdout, "Node: %s, Root: %s\n", node->value, root->value);
	}

	node->next = root;
	root->previous = node;


	/*fprintf(stdout, "Node: %s, Root: %s\n", node->value, root->value);

	fprintf(stdout, "Root: %s, Root->next: %s, Root->previous: %s\n", root->value, root->next->value, root->previous->value);
	fprintf(stdout, "Root->next->previous: %s, Root->previous->next: %s\n", root->next->previous->value, root->previous->next->value);*/

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


void func(ITEM *item, int direction)
{	
	char *name = (char*)item_name(item);
	move(20, 0);
	clrtoeol();
	mvprintw(20, 0, "Item selected is : %s", name);
}

void toggleValue(ITEM *item, int direction, List **listSpeeds, List **listDebug)
{
	int index = item_index(item);

	switch (index) {
		case 0:
			*listSpeeds = direction == UP ? (*listSpeeds)->next : (*listSpeeds)->previous;
			set_item_description(item, convertToHeapString((*listSpeeds)->value));
			break;
		case 1:
			*listDebug = direction == UP ? (*listDebug)->next : (*listDebug)->previous;
			set_item_description(item, convertToHeapString((*listDebug)->value));
			break;
		case 2:
			break;
	}
}