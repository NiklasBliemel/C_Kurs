#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Tensor *init_tensor()
{
    Tensor *out = malloc(sizeof(*out));
    out->shape = malloc(sizeof(*out->shape));
    out->stride = malloc(sizeof(*out->shape));
    out->data = malloc(0);
    out->rank = 1;
    out->num_entries = 0;
    *out->shape = 0;
    *out->stride = 1;
    return out;
}

void shape_tensor(Tensor *out, unsigned *shape, int rank)
{
    if (out->rank == rank)
    {
        int already_init = 1;
        for (int i = 0; i < out->rank; i++)
        {
            if (out->shape[i] != shape[i])
            {
                already_init = 0;
            }
        }
        if (already_init)
        {
            return;
        }
    }

    out->shape = realloc(out->shape, rank * sizeof(*shape));
    out->stride = realloc(out->stride, rank * sizeof(*shape));
    out->rank = rank;
    out->num_entries = 1;
    for (int i = 0; i < rank; i++)
    {
        out->shape[i] = shape[i];
        out->stride[rank - i - 1] = out->num_entries;
        out->num_entries *= shape[rank - i - 1];
    }
    out->data = realloc(out->data, out->num_entries * sizeof(double));
}

Tensor *subspace(unsigned *shape, int rank, double *datapoint)
{
    Tensor *out = malloc(sizeof(*out));
    out->shape = malloc(rank * sizeof(*shape));
    out->stride = malloc(rank * sizeof(*shape));
    out->rank = rank;
    out->num_entries = 1;
    for (int i = 0; i < rank; i++)
    {
        out->shape[i] = shape[i];
        out->stride[rank - i - 1] = out->num_entries;
        out->num_entries *= shape[rank - i - 1];
    }
    out->data = datapoint;
    return out;
}

void zeros(Tensor *out, unsigned *shape, int rank)
{
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 0;
    }
}

void ones(Tensor *out, unsigned *shape, int rank)
{
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 1;
    }
}

void fill(Tensor *out, unsigned *shape, int rank, double num)
{
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = num;
    }
}

void rands(Tensor *out, unsigned *shape, int rank)
{
    srand(time(NULL));
    _random();
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = _random();
    }
}

void arange(Tensor *out, unsigned range)
{
    unsigned shape[] = {range};
    shape_tensor(out, shape, 1);
    for (unsigned i = 0; i < range; i++)
    {
        out->data[i] = i;
    }
}

void linspace(Tensor *out, double start, double end, unsigned num_entries)
{
    unsigned shape[] = {num_entries};
    shape_tensor(out, shape, 1);
    for (unsigned i = 0; i < num_entries; i++)
    {
        out->data[i] = start + (double)i / (num_entries - 1) * (end - start);
    }
}

void copy(Tensor *out, Tensor *t)
{
    shape_tensor(out, t->shape, t->rank);
    for (unsigned i = 0; i < t->num_entries; i++)
    {
        out->data[i] = t->data[_reorder(t, i)];
    }
}

void eye(Tensor *out, unsigned len_diag)
{
    unsigned shape[2] = {len_diag, len_diag};
    zeros(out, shape, 2);
    for (unsigned i = 0; i < len_diag; i++)
    {
        shape[0] = i;
        shape[1] = i;
        set_element(out, shape, 2, 1);
    }
}

void outer(Tensor *out, Tensor *v1, Tensor *v2)
{

    unsigned shape[2] = {v1->num_entries, v2->num_entries};
    shape_tensor(out, shape, 2);
    for (unsigned i = 0; i < (unsigned)v1->num_entries; i++)
    {
        for (unsigned j = 0; j < (unsigned)v2->num_entries; j++)
        {
            out->data[i * out->stride[1] + j * out->stride[0]] = v1->data[i] * v2->data[j];
        }
    }
}

void house_holder(Tensor *out, Tensor *vector_1D, unsigned size)
{
    if (size < vector_1D->num_entries)
    {
        printf("vector larger than size!\n");
        return;
    }
    eye(out, size);
    double norm = dot(vector_1D, vector_1D) / 2;
    double subtract;
    unsigned offset = size - vector_1D->num_entries;

    for (unsigned i = 0; i < vector_1D->num_entries; i++)
    {
        for (unsigned j = 0; j < vector_1D->num_entries; j++)
        {
            subtract = vector_1D->data[i] * vector_1D->data[j] / norm;
            out->data[(i + offset) * out->stride[0] + (j + offset)] -= subtract;
        }
    }
}

void pop(Tensor *t)
{
    free(t->data);
    free(t->shape);
    free(t->stride);
    free(t);
}

void pop_sub(Tensor *t)
{
    free(t->shape);
    free(t->stride);
    free(t);
}
