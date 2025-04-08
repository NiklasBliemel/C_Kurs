#include <stdio.h>
#include <string.h>
#include "person.h"

void enterPerson(Person *arr, int ID, char *name, int alter, double groesse)
{
    strcpy(arr[ID].name, name);
    arr[ID].alter = alter;
    arr[ID].groesse = groesse;
}

void prinft_person_arr(Person *arr, int len_arr)
{
    printf("Name:\t");
    for (int i = 0; i < len_arr; i++)
    {
        printf("%10s", arr[i].name);
    }
    printf("\n\n");
    printf("Alter:\t");
    for (int i = 0; i < len_arr; i++)
    {
        printf("%10d", arr[i].alter);
    }
    printf("\n\n");
    printf("Größe:\t");
    for (int i = 0; i < len_arr; i++)
    {
        printf("%10.3lf", arr[i].groesse);
    }
    printf("\n\n");
}
