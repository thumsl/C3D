#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

linkedList* list_create() {
	linkedList* list = (linkedList*)malloc(sizeof(linkedList));
	list->head = NULL;
	list->tail = NULL;
}

void list_insert(linkedList* list, void *data) {
	if (list->head == NULL) {		// Change this?
		list->head = (node*)malloc(sizeof(node));
		list->tail = (node*)malloc(sizeof(node));
		list->head->data = data;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = list->head;
	}
	else {
        node* new = (node*)malloc(sizeof(node));

        new->data =data;
        new->next = NULL;
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = new;
	}
}

void append_end(); // TODO

node* list_delete_node(linkedList* list, node* N) {
	// Returns the element that was to the right of the removed element
	if (N == list->head) { // is head
		list->head = list->head->next;
		free(N);
		return list->head;
	}
	else {
		if (N== list->tail) {
			list->tail = N->prev;
			free(N);
			return NULL;
		}
		else {
			node* aux = N->next;
			N->data = N->next->data;
			N->next = N->next->next;
			free(aux);
			return N;
		}
	}
}
void list_print(node* head) {
        if (head == NULL)
                return;

        node* P = head;
        while (P != NULL) {
                printf("%p\n", P->data);
                P = P->next;
        }
}

void list_destroy(node** head) {
        node *next = (*head)->next;
        node *aux;

        while (next != NULL) {
                aux = next->next;
                free(next);
                next = aux;
        }

        free(*head);
        *head = NULL;
}
