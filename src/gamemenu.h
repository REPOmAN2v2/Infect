#ifndef MENU_H_
#define MENU_H_

#include "include.h"
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define ENTER 	10
#define DEFAULT	-1

enum {DOWN = -1, UP = 1};
typedef enum _counters {XC, YC, DOCC, INFC, NURC, SOLC, WOODC} Counters;

typedef void (*p)(void *, int, void *, void *, int []);
typedef struct _list List;

struct _list {
	char *value;
	List *next;
	List *previous;
};

void displayMenu();
List * createList(char *strings[]);
void updateUnits(int counter[]);
void fillItems(ITEM **, int [], List *, List *);
char * convertToHeapString(char *string);
void listTest(WINDOW *, List *);
void print_in_middle(WINDOW *, int, int, int, char *, chtype);
void func(ITEM *, int);
void toggleValue(ITEM *, int, List **, List **, int []);
void updateNumericValue(ITEM *item, int direction, int counters[], int index);
void set_item_description (ITEM *, const char *);

#endif