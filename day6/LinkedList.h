typedef struct list_element {
int data;
struct list_element *next;
} list_element;

list_element *NewElement(int val);

void append(list_element *head, int val);

void prepend(list_element *head, int val);

void showList(list_element * head);

int pop(list_element * head);

void clearList(list_element * head);

int len(list_element *head);