#include "person.h"

int main()
{
    Person arr[3];
    enterPerson(arr, 0, "Lisa", 43, 1.63);
    enterPerson(arr, 1, "Thomas", 35, 1.83);
    enterPerson(arr, 2, "Garfiled", 8, 1.43);
    prinft_person_arr(arr, 3);
}
