/*
 * https://github.com/REPOmAN2v2/Infect
 * 
 * Header for a custom circular linked list implementation. 
 */

#ifndef LIST_H_
#define LIST_H_

typedef struct _list List;

struct _list {
	char *value;
	List *next;
	List *previous;
};

List * createCircularLinkedList(const char * const strings[]);
void freeCircularLinkedList(List **list);

#endif