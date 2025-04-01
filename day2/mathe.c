#include <stdio.h>
#include <math.h>

int main()
{
    double start = 0;
    double end = 10;
    int n_points = 100;

    for (double x = start; x < end; x+=(end - start) / n_points)
    {
        printf("%lf %lf\n", x, cos(x));
    }
}