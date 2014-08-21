#ifndef LIST_H_
#define LIST_H_

typedef struct _list List;

struct _list {
	char *value;
	List *next;
	List *previous;
};

List * createCircularLinkedList(char *strings[]);
void freeCircularLinkedList(List **list);

#endif