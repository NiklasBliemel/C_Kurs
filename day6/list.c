#include "LinkedList.h"
#include <stdio.h>

int main()
{
    list_element *List = NewElement(4); // make

    showList(List);
    printf("size: %d\n", len(List));

    append(List, 5); // append

    showList(List);
    printf("size: %d\n", len(List));

    prepend(List, 3); // prepend

    showList(List);
    printf("size: %d\n", len(List));

    int del = pop(List); // pop

    showList(List);
    printf("size: %d\n", len(List));

    clearList(List); // clear
}