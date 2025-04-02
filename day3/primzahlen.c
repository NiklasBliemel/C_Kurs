#include <stdio.h>
#include <stdlib.h>


int main()
{
    int N;
    printf("Wähle N: "); scanf("%d", &N);

    int * arr = calloc(N + 1, sizeof(int));
    int arr_size = N + 1;
    for(int i = 0; i < arr_size; i++){arr[i] = i;}
    
    int prime = 2;
    int * out = calloc(1, sizeof(int));
    int out_size = 1;
    *out = prime;
    int fin = 0;

    while (!fin)
    {
        for(int i = 2 * prime; i < arr_size; i+=prime)
        {   
            arr[i] = 0;
        }
        for (int i = prime + 1; i < arr_size; i++)
        {
            if (arr[i] != 0)
            {
                prime = arr[i];
                out_size++;
                out = realloc(out, out_size * sizeof(*out));
                out[out_size-1] = prime;
                break;
            }
        }
        if (2 * prime >= arr_size)
        {
            fin = 1;
        }
    }
    printf("[");
    for (int i = 0; i < out_size - 1; i++)
    {
        printf("%d, ", out[i]);
    }
    printf("%d]\n", out[out_size-1]);
}