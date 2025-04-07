#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int lower = 1, upper = 99;
    int len_arr = 10;
    int arr[len_arr];
    int temp;
    int tiny_index;

    srand(time(NULL));

    for (int i = 0; i < len_arr; i++)
    {
        arr[i] = lower + rand() % (upper - lower + 1);
    }

    for (int i = 0; i < len_arr; i++)
    {
        tiny_index = i;
        for (int j = i; j < len_arr; j++)
        {
            if (arr[j] <= arr[tiny_index])
            {
                tiny_index = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[tiny_index];
        arr[tiny_index] = temp;
    }
    printf("[");
    for (int i = 0; i < len_arr - 1; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[len_arr - 1]);
}