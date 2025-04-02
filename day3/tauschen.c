#include <stdio.h>

void swap(double **a, double **b);

int main()
{
    double a = 1.3;
    double b = 7.7;
    double *a_p = &a;
    double *b_p = &b;
    printf("a = %lf\tb = %lf\n", *a_p, *b_p);
    
    swap(&a_p, &b_p);

    printf("a = %lf\tb = %lf\n", *a_p, *b_p);
}

void swap(double **a, double **b)
{
    double *temp = *a;
    *a = *b;
    *b = temp;
}