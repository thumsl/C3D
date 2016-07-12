#ifndef LIST_H
#define LIST_H

typedef struct node {
        void* data;
        struct node* prev;
        struct node* next;
} node;

node* list_create(void* data);
void list_insert(node** head, void *data);
void list_delete(node* N);
void list_destroy(node** head);
void list_print(node* head);

#endif //LIST_H