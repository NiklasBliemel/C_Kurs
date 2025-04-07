#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int comp(const void *elem1, const void *elem2)
{
    int f = *((int *)elem1);
    int s = *((int *)elem2);
    if (f > s)
        return 1;
    if (f < s)
        return -1;
    return 0;
}

void print_arr(int *a, int len_arr)
{
    printf("[");
    for (int i = 0; i < len_arr - 1; i++)
    {
        printf("%d, ", a[i]);
    }
    printf("%d]\n", a[len_arr - 1]);
}

void merge(int *source_arr, int *copy_arr, int n_a, int n_b)
{
    int i = 0, j = 0;
    while (i < n_a && j < n_b)
    {
        if (source_arr[i] < source_arr[n_a + j])
        {
            copy_arr[i + j] = source_arr[i];
            i++;
        }
        else
        {
            copy_arr[i + j] = source_arr[n_a + j];
            j++;
        }
    }
    while (i + j < n_a + n_b)
    {
        if (i < n_a)
        {
            copy_arr[i + j] = source_arr[i];
            i++;
        }
        else
        {
            copy_arr[i + j] = source_arr[n_a + j];
            j++;
        }
    }
}

void merge_sort(int *list1, int n)
{
    if (n == 1)
    {
        return;
    }

    int *list2 = malloc(n * sizeof(*list1));
    int n_sweeps = (int)log2((double)n);
    if (n > (int)(pow(2, (double)n_sweeps)))
    {
        n_sweeps++;
    }
    int n_megre = 1;
    for (int i = 0; i < n_sweeps; i++)
    {
        n_megre *= 2;
        if (i % 2 == 0)
        {
            for (int j = 0; j < n; j += n_megre)
            {
                if ((n - j) / n_megre > 0)
                {
                    merge(&list1[j], &list2[j], n_megre / 2, n_megre / 2);
                }
                else if ((n - j) / (n_megre / 2) > 0)
                {
                    merge(&list1[j], &list2[j], n_megre / 2, (n - j - n_megre / 2));
                }
            }
        }
        else
        {
            for (int j = 0; j < n; j += n_megre)
            {
                if ((n - j) / n_megre > 0)
                {
                    merge(&list2[j], &list1[j], n_megre / 2, n_megre / 2);
                }
                else if ((n - j) / (n_megre / 2) > 0)
                {
                    merge(&list2[j], &list1[j], n_megre / 2, (n - j - n_megre / 2));
                }
            }
        }
    }
    if (n_sweeps % 2 == 1)
    {
        for (int i = 0; i < n; i++)
        {
            list1[i] = list2[i];
        }
    }
    free(list2);
}

int main()
{
    int lower = 1, upper = 99;
    int len_arr = 1e6;
    int arr[len_arr];
    int copy_arr[len_arr];

    srand(time(NULL));

    for (int i = 0; i < len_arr; i++)
    {
        arr[i] = lower + rand() % (upper - lower + 1);
        copy_arr[i] = arr[i];
    }
    clock_t begin = clock();
    merge_sort(arr, len_arr);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Mergesort in %lf sec\n", time_spent);
    begin = clock();
    qsort(copy_arr, len_arr, sizeof(*copy_arr), comp);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Qsort in %lf sec\n", time_spent);
    printf("Mergesort Arr:\n");
}
