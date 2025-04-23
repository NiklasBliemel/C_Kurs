#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

void reshape(Tensor *tensor, unsigned int *shape, int len_shape)
{
    unsigned int check = 1;
    int fill_index = -1;
    for (int i = 0; i < len_shape; i++)
    {
        if (shape[i] != (unsigned int)-1)
        {
            check *= shape[i];
        }
        else
        {
            fill_index = i;
        }
    }
    if (fill_index != -1)
    {
        shape[fill_index] = tensor->num_entries / check;
        check *= shape[fill_index];
    }
    if (check != tensor->num_entries)
    {
        printf("ERROR 3\n");
        return;
    }
    tensor->shape = realloc(tensor->shape, len_shape * sizeof(int));
    *tensor->shape = *shape;
    unsigned int temp = tensor->stride[tensor->len_shape - 1];
    tensor->stride = realloc(tensor->stride, len_shape * sizeof(int));
    tensor->stride[len_shape - 1] = temp;
    tensor->len_shape = len_shape;
    for (int i = len_shape - 1; i > 0; i--)
    {
        tensor->shape[i] = shape[i];
        tensor->stride[i - 1] = (shape[i] * tensor->stride[i]) % (tensor->num_entries - 1);
    }
}

void permute(Tensor *tensor, int *permutation, int len_permutation)
{
    if (len_permutation != tensor->len_shape)
    {
        printf("ERROR 1\n");
        return;
    }
    int *copy_shape = malloc(len_permutation * sizeof(int));
    int *copy_stride = malloc(len_permutation * sizeof(int));
    for (int i = 0; i < len_permutation; i++)
    {
        copy_shape[i] = tensor->shape[i];
        copy_stride[i] = tensor->stride[i];
        if (permutation[i] >= len_permutation)
        {
            printf("ERROR 2\n");
            return;
        }
    }
    for (int i = 0; i < len_permutation; i++)
    {
        tensor->shape[i] = copy_shape[permutation[i]];
        tensor->stride[i] = copy_stride[permutation[i]];
    }
}

void flat(Tensor *t)
{
    unsigned int shape[] = {-1};
    reshape(t, shape, 1);
}
