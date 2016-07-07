#ifndef LIST_H
#define LIST_H

typedef struct node {
	void* data;
	struct node* prev;
	struct node* next;
} node;

node* createList(void* data);
void append_anywhere(node* head, void *data);
void delete(node* N);

#endif //LIST_H