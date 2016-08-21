#ifndef LIST_H
#define LIST_H

typedef struct node_s {
    void* data;
    struct node_s* prev;
    struct node_s* next;
} node;

typedef struct linkedList_s {
	node* head;
	node* tail;
} linkedList;

linkedList* list_create();
node* list_delete_node(linkedList* head, node* N);
void list_insert(linkedList* head, void *data);
void list_destroy(linkedList *list);

#endif //LIST_H