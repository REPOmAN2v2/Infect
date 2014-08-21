#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List * createCircularLinkedList(char *strings[])
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