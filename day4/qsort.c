#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int checksum(int num)
{
    int out = 0;
    while (num > 10)
    {
        out += num % 10;
        num /= 10;
    }
    return out + num % 10;
}

int comp_checksum(const void *a, const void *b)
{
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    arg1 = checksum(arg1);
    arg2 = checksum(arg2);
    if (arg1 < arg2)
        return -1;
    if (arg1 > arg2)
        return 1;
    return 0;
}

void prinf_arr(int *a, int len_arr)
{
    printf("[");
    for (int i = 0; i < len_arr - 1; i++)
    {
        printf("%d, ", a[i]);
    }
    printf("%d]\n", a[len_arr - 1]);
}

int main()
{
    int lower = 1, upper = 99;
    int len_arr = 10;
    int arr[len_arr];

    srand(time(NULL));

    for (int i = 0; i < len_arr; i++)
    {
        arr[i] = lower + rand() % (upper - lower + 1);
    }

    qsort(arr, len_arr, sizeof(*arr), comp_checksum);
    prinf_arr(arr, len_arr);
}
