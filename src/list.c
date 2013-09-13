#include <stdlib.h>
#include "list.h"


list *list_append(list *head, void *data)
{
        list *e = malloc(sizeof(list));
        
        e->data = data;
        e->next = NULL;
        
        if (head == NULL) {
                return e;
        }

        list *tail = head;

        while (tail->next != NULL) {
                tail = tail->next;
        }

        tail->next = e;

        return head;
}

list *list_prepend(list *head, void *data)
{
        list *e = malloc(sizeof(list));
        
        e->data = data;

        if (head == NULL) {
                e->next = NULL;
        } else {
                e->next = head;
        }

        return e;
}

list *list_remove(list *head, list *e)
{
        if (head == e) {
                head = e->next;
        } else {
                list *p = head;

                while (p->next != e) {
                        p = p->next;
                }

                p->next = e->next;
        }
        
        free(e);

        return head;
}

// TEST CODE
/*
int main()
{
        list *a = NULL;

        a = list_prepend(a, (void *) 1);
        a = list_prepend(a, (void *) 2);
        a = list_prepend(a, (void *) 3);
        a = list_prepend(a, (void *) 4);
        a = list_prepend(a, (void *) 5);


        //a = list_remove(a, a);

        while (a != NULL) {
                printf("%d\n", (int) a->data);
                a = a->next;
        }
        
        return 0;
}
*/
