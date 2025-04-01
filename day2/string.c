#include <stdio.h>

int main()
{
    char string[1024];
    printf("Bitte geben Sie den String ein!: ");
    scanf("%s", string);
    for (int i = 0; i < 1024; i++)
    {
        if (64 < (int) string[i] && (int) string[i] < 91)
        {
            string[i] += 32;
        }
    }
    printf("%s\n", string);
}