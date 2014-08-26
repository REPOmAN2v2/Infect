/*
 * https://github.com/REPOmAN2v2/Infect
 * 
 * This simply deals with circular linked lists. The code is currently not 
 * portable since the node values are implementation specific.
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * createCircularLinkedList() creates a circular linked list from an array of
 * of strings.
 *
 * It takes in as a parameter an array of strings and fills each node's value
 * by duplicating the strings in the array. It returns a pointer to the root 
 * node.
 *
 * Freeing the values and the nodes is left to the caller, by calling 
 * freeCircularLinkedList().
 * 
 * Remark: createCircularLinkedList() needs to be called by casting the two 
 * lists with (const char * const *) to squash a harmless (in this case) 
 * warning from GCC.
 * See http://c-faq.com/ansi/constmismatch.html
 */
List * createCircularLinkedList(const char * const strings[])
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
	}

	node->next = root;
	root->previous = node;

	return root;
}

/*
 * freeCircularLinkedList() frees a circular linked list with a heap allocated
 * value.
 *
 * It takes in as a parameter a double pointer to a list node then frees every 
 * node and their value.
 */
void freeCircularLinkedList(List **list)
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