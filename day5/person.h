typedef struct
{
    char name[50];
    int alter;
    double groesse;
} Person;

void enterPerson(Person *arr, int ID, char *name, int alter, double groesse);

void prinft_person_arr(Person *arr, int len_arr);