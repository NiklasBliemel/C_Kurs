#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

void print_tensor(Tensor *t)
{
    if (t->num_entries == 0)
    {
        printf("[]");
        return;
    }

    int extra_print;
    for (unsigned flat_index = 0; flat_index < t->num_entries - 1; flat_index++)
    {
        if (flat_index == 0)
        {
            extra_print = 0;
        }
        else
        {
            extra_print = -1;
        }

        for (int i = 0; i < t->rank; i++)
        {
            if (flat_index % t->stride[i] == 0)
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
            if (flat_index != 0)
            {
                for (int i = 0; i < extra_print; i++)
                {
                    printf("]");
                }
                printf(",");
                if (flat_index != t->num_entries - 1)
                {
                    for (int i = 0; i < extra_print; i++)
                    {
                        printf("\n");
                    }
                }
            }
            for (int i = 0; i < t->rank; i++)
            {
                if (i < t->rank - extra_print)
                {
                    printf(" ");
                }
                else
                {
                    printf("[");
                }
            }
        }
        printf("%4.3lf", t->data[_reorder(t, flat_index)]);
    }
    printf(", %4.3lf", t->data[t->num_entries - 1]);
    for (int i = 0; i < t->rank; i++)
    {
        printf("]");
    }
    printf("\n");
}

void print_shape(Tensor *t)
{
    printf("Shape: [%d", *t->shape);
    for (int i = 1; i < t->rank; i++)
    {
        printf(", %d", t->shape[i]);
    }
    printf("]\n");
}

void print_stride(Tensor *t)
{
    printf("Stride: [%d", *t->stride);
    for (int i = 1; i < t->rank; i++)
    {
        printf(", %d", t->stride[i]);
    }
    printf("]\n");
}
