#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

void reshape(Tensor *t, unsigned *shape, int rank)
{
    Tensor *new_t = init_tensor();
    shape_tensor(new_t, shape, rank);
    if (new_t->num_entries != t->num_entries)
    {
        pop(new_t);
        printf("Num entries don't match!!\n");
        return;
    }
    for (unsigned i = 0; i < t->num_entries; i++)
    {
        new_t->data[i] = t->data[_reorder(t, i)];
    }
    free(t->data);
    free(t->shape);
    free(t->stride);
    t->data = new_t->data;
    t->shape = new_t->shape;
    t->stride = new_t->stride;
    t->rank = rank;
    pop(new_t);
}

void view(Tensor *t, unsigned *shape, int rank)
{
    unsigned check_num_entries = _fill_shape(t, shape, rank);
    if (t->num_entries != check_num_entries)
    {
        printf("new number of entries does not Match old!!\n");
        return;
    }

    unsigned *new_shape = malloc(rank * sizeof(unsigned));
    *new_shape = *shape;
    unsigned *new_stride = malloc(rank * sizeof(unsigned));
    new_stride[rank - 1] = t->stride[t->rank - 1];

    for (int i = rank - 1; i > 0; i--)
    {
        new_shape[i] = shape[i];
        new_stride[i - 1] = (shape[i] * new_stride[i]) % (t->num_entries - 1);
        if (new_shape[i] * new_stride[i] > check_num_entries)
        {
            printf("view size is not compatible with input tensor's size and stride (at least one dimension spans across two contiguous subspaces). Use .reshape(...) instead.\n");
            free(new_shape);
            free(new_stride);
            return;
        }
    }
    if (*new_shape * *new_stride > check_num_entries)
    {
        printf("view size is not compatible with input tensor's size and stride (at least one dimension spans across two contiguous subspaces). Use .reshape(...) instead.\n");
        free(new_shape);
        free(new_stride);
        return;
    }
    free(t->shape);
    free(t->stride);
    t->shape = new_shape;
    t->stride = new_stride;
    t->rank = rank;
}

void permute(Tensor *t, int *permutation, int len_permutation)
{
    if (len_permutation != t->rank)
    {
        printf("permutation must be size of tensor rank!\n");
        return;
    }
    unsigned *new_shape = malloc(len_permutation * sizeof(int));
    unsigned *new_stride = malloc(len_permutation * sizeof(int));
    for (int i = 0; i < len_permutation; i++)
    {
        new_shape[i] = t->shape[i];
        new_stride[i] = t->stride[i];
        if (permutation[i] >= len_permutation)
        {
            printf("Max permutation index = %d\n", t->rank - 1);
            free(new_shape);
            free(new_stride);
            return;
        }
    }
    free(t->shape);
    free(t->stride);
    t->shape = new_shape;
    t->stride = new_stride;
}

void transpose(Tensor *t)
{
    if (t->rank < 2)
    {
        printf("Rank too small!\n");
        return;
    }
    unsigned temp = t->shape[t->rank - 1];
    t->shape[t->rank - 1] = t->shape[t->rank - 2];
    t->shape[t->rank - 2] = temp;
    temp = t->stride[t->rank - 1];
    t->stride[t->rank - 1] = t->stride[t->rank - 2];
    t->stride[t->rank - 2] = temp;
}

void flat(Tensor *t)
{
    unsigned shape[] = {-1};
    reshape(t, shape, 1);
}
