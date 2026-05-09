#include <stdlib.h>
#include "../include/list.h"

linkedList *list_create()
{
	linkedList *list = malloc(sizeof(linkedList));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void list_insert(linkedList *list, void *data)
{
	if (list->head == NULL) {
		node *new = malloc(sizeof(node));
		new->data = data;
		new->prev = NULL;
		new->next = NULL;
		list->head = new;
		list->tail = new;
	} else {
		node *new = malloc(sizeof(node));

		new->data = data;
		new->next = NULL;
		new->prev = list->tail;
		list->tail->next = new;
		list->tail = new;
	}
}

node *list_delete_node(linkedList *list, node *N)
{
	// Returns the element that was to the right of the removed element
	if (N == list->head) { // is head
		list->head = list->head->next;
		if (list->head == NULL) {
			list->tail = NULL;
		} else {
			list->head->prev = NULL;
		}
		free(N);
		return list->head;
	} else {
		if (N == list->tail) {
			list->tail = N->prev;
			list->tail->next = NULL;
			free(N);
			return NULL;
		} else {
			node *next = N->next;
			N->prev->next = N->next;
			N->next->prev = N->prev;
			free(N);
			return next;
		}
	}
}

void list_destroy(linkedList *list)
{
	node *aux = list->head;
	node *prev;
	while (aux != NULL) {
		prev = aux;
		aux = aux->next;
		free(prev->data);
		free(prev);
	}

	free(list);
}
