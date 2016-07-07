#include "../include/list.h"
#include <stdlib.h>

node* createList(void* data) {
	node* ret = (node*)malloc(sizeof(node));

	ret -> prev = NULL;
	ret -> next = NULL;

	return ret;
}

void append_anywhere(node* head, void *data) {
	node* new = (node*)malloc(sizeof(node));

	new->data = data;

	new->next = head->next;
	new->prev = head;

	if (head->next != NULL) {
		head->next->prev = new;
		head->next = new;
	}
}

void append_end(); // TODO

void delete_node(node* N) {
	if (N->prev == NULL) { // is Head
		node* del = N;
		N = N->prev;
		free(del);
	}
	else if (N->next == NULL) {
		N->prev->next = NULL;
		free(N);
	}
	else {
		N->prev->next = N->next;
		N->next->prev = N->prev;
		free(N);
	}
}