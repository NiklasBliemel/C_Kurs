#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

void print_tensor(Tensor *tensor)
{
    int extra_print;
    unsigned int temp;

    for (int i = 0; i < tensor->len_shape; i++)
    {
        printf("[");
    }
    printf("%4.3lf", *tensor->data);

    for (unsigned int flat_index = 1; flat_index < tensor->num_entries - 1; flat_index++)
    {
        extra_print = 0;
        temp = 1;
        for (int i = 0; i < tensor->len_shape; i++)
        {
            temp *= tensor->shape[tensor->len_shape - 1 - i];
            if (flat_index % temp == 0)
            {
                extra_print++;
            }
        }
        if (extra_print == 0)
        {
            printf(" ,");
        }
        else
        {
            for (int i = 0; i < extra_print; i++)
            {
                printf("]");
            }
            printf(",");
            for (int i = 0; i < extra_print; i++)
            {
                printf("\n");
            }
            for (int i = 0; i < tensor->len_shape; i++)
            {
                if (i < tensor->len_shape - extra_print)
                {
                    printf(" ");
                }
                else
                {
                    printf("[");
                }
            }
        }

        printf("%4.3lf", tensor->data[_reorder_index(tensor, flat_index, -1)]);
    }
    if (tensor->num_entries > 1)
    {
        printf(", %4.3lf", tensor->data[tensor->num_entries - 1]);
    }
    for (int i = 0; i < tensor->len_shape; i++)
    {
        printf("]");
    }
    printf("\n");
}

void print_shape(Tensor *tensor)
{
    printf("Shape: [%d", *tensor->shape);
    for (int i = 1; i < tensor->len_shape; i++)
    {
        printf(", %d", tensor->shape[i]);
    }
    printf("]\n");
}

void print_stride(Tensor *tensor)
{
    printf("Stride: [%d", *tensor->stride);
    for (int i = 1; i < tensor->len_shape; i++)
    {
        printf(", %d", tensor->stride[i]);
    }
    printf("]\n");
}
