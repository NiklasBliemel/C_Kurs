#include <stdio.h>
#include <stdlib.h>

int fibunacci(int N)
{
    if (N < 1)
    {
        return 0;
    }
    else if (N == 1)
    {
        return 1;
    }

    {
        return fibunacci(N - 1) + fibunacci(N - 2);
    }
}

int fibunacci_loop(int N)
{
    if (N < 1)
    {
        return 0;
    }
    else if (N == 1)
    {
        return 1;
    }
    int temp = 0;
    int f_last = 1;
    int f_last_last = 0;
    for (int i = 2; i <= N; i++)
    {
        temp = f_last;
        f_last += f_last_last;
        f_last_last = temp;
    }
    return f_last;
}

int main(int argc, char **argv)
{
    int N = 5;

    if (argc > 1)
    {
        N = atoi(argv[1]);
    }
    printf("Fibinacci(%d) = %d\n", N, fibunacci(N));
    printf("Fibinacci(%d) = %d\n", N, fibunacci_loop(N));
}
