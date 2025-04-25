#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

// replaces -1 entry with remaining number of entries of Tensor *t, if possible
unsigned _fill_shape(Tensor *t, unsigned *shape, int rank)
{
    unsigned fill_index = -1;
    unsigned product = 1;
    int counter = 0;
    for (int i = 0; i < rank; i++)
    {
        if (shape[i] != (unsigned)-1)
        {
            product *= shape[i];
        }
        else
        {
            if (counter > 0)
            {
                printf("To many -1 in new shape!\n");
                return -1;
            }
            counter++;
            fill_index = i;
        }
    }
    if (fill_index != (unsigned)-1)
    {
        shape[fill_index] = t->num_entries / product;
        product *= shape[fill_index];
    }
    return product;
}

// calculates correct index for t.data array using the stride
unsigned _flatten_index(Tensor *t, unsigned *index_list, int len_list)
{
    if (t->rank != len_list)
    {
        printf("Index List (%d) does not match Tensor shape (%d)\n", len_list, t->rank);
        return -1;
    }
    unsigned index = 0;
    for (int i = 0; i < t->rank; i++)
    {
        if (t->shape[i] < index_list[i])
        {
            printf("%d. Index (%d) is out of range (%d)\n", i, index_list[i], t->shape[i]);
            return -1;
        }
        else
        {
            index += index_list[i] * t->stride[i];
        }
    }
    return index;
}

// used to iterate through the complete t.data array in the right order defined by the stride
unsigned _reorder(Tensor *t, unsigned flat_index)
{
    if (flat_index == t->num_entries - 1)
    {
        return flat_index;
    }
    return flat_index * t->stride[t->rank - 1] % (t->num_entries - 1);
}

// used to iterate through all dimensions upper the 2nd. Used for multi dimensional matmul
unsigned _reorder_three(Tensor *t, unsigned residual, unsigned i1, unsigned i2, unsigned i3)
{
    if ((i1 % residual) == residual - 1 && i2 == t->shape[t->rank - 2] - 1 && i3 == t->shape[t->rank - 1] - 1)
    {
        return t->num_entries - 1;
    }
    unsigned out = 0;
    out += i2 * t->stride[t->rank - 2];
    out += i3 * t->stride[t->rank - 1];
    if (t->rank > 2)
    {
        out += (i1 % residual) * t->stride[t->rank - 3];
    }
    return out % (t->num_entries - 1);
}

// returns random double value between -1 and 1 (not the best rng but sufficient for testing propuse)
double _random()
{
    return ((double)rand() * (2)) / (double)RAND_MAX - 1;
}
