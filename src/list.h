/*
 * Single linked list implementation.
 */

#define EMPTY_LIST NULL

// List entry. List object simply represents as pointer to list's head.
typedef struct list {
        struct list *next;
        void *data;
} list;

// Append new item to the beginning of the list.
// If list argument is NULL then new one-item list will be created.
list *list_prepend(list *head, void *data);

// Append new item to the end of the list.
// If list argument is NULL then new one-item list will be created.
list *list_append(list *head, void *data);

// Removes item from the list and returns new head.
list *list_remove(list *head, list *e);
