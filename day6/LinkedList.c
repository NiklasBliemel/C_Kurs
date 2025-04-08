#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>

list_element *NewElement(int val)
{
    list_element *new = malloc(sizeof(*new));
    new->data = val;
    new->next = NULL;
    return new;
}

void append(list_element *head, int val)
{
    list_element *new = NewElement(val);
    while (head->next)
    {
        head = head->next;
    }
    head->next = new;
}

void prepend(list_element *head, int val)
{
    list_element *new = NewElement(head->data);
    new->next = head->next;
    head->next = new;
    head->data = val;
}

void showList(list_element *head)
{
    printf("[");
    while (head->next)
    {
        printf("%d, ", head->data);
        head = head->next;
    }
    printf("%d]\n", head->data);
}

int pop(list_element *head)
{
    int ret = head->data;
    if (head->next == NULL)
    {
        free(head);
    }
    else
    {
        head->data = head->next->data;
        list_element *del = head->next;
        head->next = head->next->next;
        free(del);
    }
    return ret;
}

void clearList(list_element *head)
{
    while (head->next)
    {
        pop(head);
    }
    pop(head);
}

int len(list_element *head)
{
    int size = 1;
    while (head->next)
    {
        size++;
        head = head->next;
    }
    return size;
}