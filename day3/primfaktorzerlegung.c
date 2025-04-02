#include <stdio.h>
#include <stdlib.h>

int *append(int *arr, int num)
{
    *arr += 1;
    arr = realloc(arr, (*arr + 1) * sizeof(*arr));
    arr[*arr] = num;
    return arr;
}

int *get_primes(int N)
{
    int *arr = calloc(N + 1, sizeof(int));
    *arr = N;
    for (int i = 1; i <= *arr; i++)
    {
        arr[i] = i;
    }

    int prime = 2;
    int *out = calloc(2, sizeof(int));
    *out = 1;
    out[1] = prime;
    int fin = 0;

    while (!fin)
    {
        for (int i = 2 * prime; i <= *arr; i += prime)
        {
            arr[i] = 0;
        }
        fin = 1;
        for (int i = prime + 1; i <= *arr; i++)
        {
            if (arr[i] != 0)
            {
                fin = 0;
                prime = arr[i];
                out = append(out, prime);
                break;
            }
        }
    }
    return out;
}

int main()
{
    int N;
    printf("Wähle N: ");
    scanf("%d", &N);

    int *primes = get_primes(N);

    int *out = calloc(1, sizeof(int));
    *out = 0;

    for (int i = 1; i <= *primes; i++)
    {
        while (N % primes[i] == 0)
        {
            N /= primes[i];
            out = append(out, primes[i]);
        }
        if (primes[i] * primes[i] > N)
        {
            out = append(out, N);
            break;
        }
    }
    int check = out[1];
    printf("%d", check);
    for (int i = 2; i <= *out; i++)
    {
        printf(" * %d", out[i]);
        check *= out[i];
    }
    printf(" = %d\n", check);
}