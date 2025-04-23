#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int _flatten_index(Tensor *tensor, unsigned int *index_list, int len_list)
{
    if (tensor->len_shape != len_list)
    {
        printf("Index List (%d) does not match Tensor shape (%d)\n", *index_list, *tensor->shape);
        return -1;
    }
    unsigned int index = 0;
    for (int i = 0; i <= tensor->len_shape; i++)
    {
        if (tensor->shape[i] < index_list[i])
        {
            printf("%d. Index (%d) is out of range (%d)\n", i, index_list[i], tensor->shape[i]);
            index = -1;
            break;
        }
        else
        {
            index += index_list[i] * tensor->stride[i];
        }
    }
    return index % tensor->num_entries;
}

unsigned int _reorder_index(Tensor *tensor, unsigned int index, int dim)
{
    if (index == tensor->num_entries - 1)
    {
        return tensor->num_entries - 1;
    }
    if (dim < 0)
    {
        return index * tensor->stride[tensor->len_shape + dim] % (tensor->num_entries - 1);
    }
    return index * tensor->stride[dim] % (tensor->num_entries - 1);
}

unsigned int _reorder_three(Tensor *tensor, unsigned int residual, unsigned int i1, unsigned int i2, unsigned int i3)
{
    if ((i1 % residual) == residual - 1 && i2 == tensor->shape[tensor->len_shape - 2] - 1 && i3 == tensor->shape[tensor->len_shape - 1] - 1)
    {
        return tensor->num_entries - 1;
    }
    unsigned int out = 0;
    out += i2 * tensor->stride[tensor->len_shape - 2];
    out += i3 * tensor->stride[tensor->len_shape - 1];
    if (tensor->len_shape > 2)
    {
        out += (i1 % residual) * tensor->stride[tensor->len_shape - 3];
    }
    return out % (tensor->num_entries - 1);
}

double abs(double num)
{
    if (num < 0)
    {
        return -num;
    }
    return num;
}
