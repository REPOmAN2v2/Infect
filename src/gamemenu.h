#ifndef MENU_H_
#define MENU_H_

#include "include.h"
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define ENTER 	10

enum {DOWN = -1, UP = 1};
typedef enum _counters {XC, YC, DOCC, INFC, NURC, SOLC, WOODC} Counters;

typedef struct _list List;

struct _list {
	char *value;
	List *next;
	List *previous;
};

void displayMenu();
void eventLoop(MENU *, WINDOW *, ITEM **, List *, List *, int []);
List * createList(char *strings[]);
void freeList(List **);
void quitMenu(MENU *, ITEM **, List *, List *);
void updateUnits(int counter[]);
void updateUnitsDisplay(ITEM **, int []);
void fillItems(ITEM **, int [], List *, List *);
char * convertToHeapString(char *string);
void print_in_middle(WINDOW *, int, int, int, char *, chtype);
void toggleValue(ITEM *, int, List **, List **, int []);
void updateNumericValue(ITEM *item, int direction, int counters[], int index);
void set_item_description (ITEM *, const char *);

#endif