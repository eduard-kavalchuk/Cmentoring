// Implement Linked list and insert and delete operations for it.

#include <stdio.h>
#include <stdlib.h>

struct LinkedList {
    int value;
    struct LinkedList *next;
};

struct LinkedListHead {
    unsigned int size;
    struct LinkedList *first;
    struct LinkedList *last;
};

struct LinkedListHead *init(void);
void insert(struct LinkedListHead *listHead, int value);
void deleteAll(struct LinkedListHead *listHead);

void printList(struct LinkedListHead *listHead);

int main(void)
{
    struct LinkedListHead *listHead = init();
    for(int i = 0; i < 10; i++) {
        insert(listHead, i);
    }
    printList(listHead);
    
    return 0;
}

struct LinkedListHead *init(void)
{
    struct LinkedListHead *head = (struct LinkedListHead *) malloc(sizeof(struct LinkedListHead));
    head->size = 0;
    head->first = NULL;
    head->last = NULL;
    return head;
}

void insert(struct LinkedListHead *listHead, int value)
{
    struct LinkedList *list = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    list->value = value;
    list->next = NULL;
    if(listHead->last != NULL) {
        listHead->last->next = list;
    }
    else {
        listHead->first = list;
    }
    
    listHead->last = list;
    listHead->size++;
}

void printList(struct LinkedListHead *listHead)
{
    struct LinkedList *ptr = listHead->first;
    while(ptr != NULL) {
        printf("%d ", ptr->value);
        ptr = ptr->next;
    }
    
    printf("\n");
}



