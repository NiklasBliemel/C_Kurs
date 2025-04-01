#include <stdio.h>

int main()
{
    double out = 0;
    double vector_a[7] = {3, 2, 1, 5, 7, 2, -1};
    double vector_b[7] = {-7, 3, 7, 5, 6, 8, 1};
    if (sizeof(vector_a) == sizeof(vector_b))
    {
        for (int i = 0; i < sizeof(vector_a) / sizeof(vector_a[0]); i++)
        {
            out += vector_a[i] * vector_b[i];
        }
        printf("Skalarprodukt: %lf\n", out);
    }
    else
    {
        printf("Vectors not of same size!\n");
    }
}