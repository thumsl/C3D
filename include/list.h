#ifndef LIST_H
#define LIST_H

typedef struct node {
    void* data;
    struct node* prev;
    struct node* next;
} node;

typedef struct linkedList {
	node* head;
	node* tail;
} linkedList;

linkedList* list_create();
node* list_delete_node(linkedList* head, node* N);
void list_insert(linkedList* head, void *data);
void list_destroy(linkedList *list);

#endif //LIST_H
