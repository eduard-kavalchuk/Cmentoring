// Implement Linked list and append and delete operations for it.

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
void append(struct LinkedListHead *listHead, int value);
void clear(struct LinkedListHead *listHead);
void insert(struct LinkedListHead *listHead, int position, int value);
void printList(struct LinkedListHead *listHead);
void delete(struct LinkedListHead *listHead, int value);


int main(void)
{
    struct LinkedListHead *listHead = init();
    printf("Create a list of integers from 0 to 9:\n");
    for(int i = 0; i < 10; i++) {
        append(listHead, i);
    }
    printList(listHead);
    
    printf("Insert value 11 in position 4:\n");
    insert(listHead, 4, 11);
    printList(listHead);
    
    printf("Insert value 12 to the head of the list:\n");
    insert(listHead, 0, 12);
    printList(listHead);
    
    printf("Insert value 13 to the end of the list:\n");
    insert(listHead, listHead->size, 13);
    printList(listHead);
    
    printf("Try to delete a non-existent value (15):\n");
    delete(listHead, 15);
    printList(listHead);
    
    printf("Delete first value (12):\n");
    delete(listHead, 12);
    printList(listHead);
    
    printf("Delete last value (13):\n");
    delete(listHead, 13);
    printList(listHead);
    
    printf("Delete middle value (11):\n");
    delete(listHead, 11);
    printList(listHead);
    
    clear(listHead);
    
    return 0;
}

void delete(struct LinkedListHead *listHead, int value)
{   
    struct LinkedList *prev = listHead->first, *curr = listHead->first;

    while (curr != NULL) {
        if (curr->value == value) {
            if (curr == listHead->first) {
                listHead->first = curr->next;
            }
            else if (curr == listHead->last) {
                listHead->last = prev;
                prev->next = NULL;
            }
            else {
                prev->next = curr->next;
            }
            free(curr);
            listHead->size--;
            return;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void insert(struct LinkedListHead *listHead, int position, int value)
{   
    if (listHead->size < position)
        return;
    
    struct LinkedList *list = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    list->value = value;
    
    if (listHead->size == position) {
        list->next = NULL;
        if(listHead->last != NULL) {
            listHead->last->next = list;
        }
        else {
            listHead->first = list;
        }
        listHead->last = list;
    }
    else if (position == 0) {
        list->next = listHead->first;
        listHead->first = list;
    }
    else {
        struct LinkedList *tmp = listHead->first;
        for (int i = 0; i < position - 1; i++)
            tmp = tmp->next;
        list->next = tmp->next;
        tmp->next = list;
    }
    
    listHead->size++;
}

struct LinkedListHead *init(void)
{
    struct LinkedListHead *head = (struct LinkedListHead *) malloc(sizeof(struct LinkedListHead));
    head->size = 0;
    head->first = NULL;
    head->last = NULL;
    return head;
}

void append(struct LinkedListHead *listHead, int value)
{
    insert(listHead, listHead->size, value);
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

void clear(struct LinkedListHead *listHead)
{
    struct LinkedList *ptr = listHead->first;
    while(ptr != NULL) {
        struct LinkedList *tmp = ptr;
        free(ptr);
        ptr = tmp->next;
    }
}

