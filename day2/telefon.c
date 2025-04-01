#include <stdio.h>

int main()
{
    int array_a[9];
    int array_b[3][3];
    for (int i = 0; i < 9; ++i)
    {
        array_a[i] = i + 1;
        array_b[i / 3][i % 3] = i + 1;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d\t", array_b[i][j]);
        }
    printf("\n\n");    
    }
}