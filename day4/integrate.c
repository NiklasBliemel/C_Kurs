#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double integrate(
    double (*func)(double),
    double from,
    double to,
    int N)
{
    double h_N = (to - from) / N;
    double x_i;
    double out = 0;
    for (int i = 0; i <= N; i++)
    {
        x_i = from + i * h_N;
        if (i == 0 || i == N) // simpson's rule
        {
            out += func(x_i) / 3;
        }
        else if (i % 2 == 1)
        {
            out += func(x_i) * 4 / 3;
        }
        else
        {
            out += func(x_i) * 2 / 3;
        }
    }
    return out * h_N;
}

double test_func(double x)
{
    return x * x * x / 3;
}

int main(int argc, char **argv)
{
    double from = 0, to = 1;
    int N = 10000;

    if (argc > 1)
    {
        N = atoi(argv[1]);
    }
    printf("Integral = %lf\n", integrate(exp, from, to, N));
}
