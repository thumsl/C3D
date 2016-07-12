#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

node* list_create(void* data) {
        node* ret = (node*)malloc(sizeof(node));

        ret -> data = data;
        ret -> prev = NULL;
        ret -> next = NULL;

        return ret;
}

void list_insert(node** head, void *data) {
	if ((*head) == NULL) {		// Change this?
		(*head) = (node*)malloc(sizeof(node));
		(*head)->data = data;
		(*head)->prev = NULL;
		(*head)->next = NULL;
	}
	else {
        node* new = (node*)malloc(sizeof(node));

        new->data = data;
        new->next = (*head)->next;
        new->prev = (*head);

        (*head)->next = new;

		if ((*head)->next != NULL)
			(*head)->next->prev = new;
	}
}

void append_end(); // TODO

void list_delete_node(node* N) {
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
