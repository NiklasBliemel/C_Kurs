#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Tensor *init_tensor()
{
    Tensor *out = malloc(sizeof(*out));
    out->shape = malloc(sizeof(*out->shape));
    out->stride = malloc(sizeof(*out->shape));
    out->data = malloc(sizeof(double));
    out->len_shape = 1;
    out->num_entries = 1;
    *out->shape = 1;
    *out->stride = 1;
    return out;
}

void shape_tensor(Tensor *out, unsigned *shape, int len_shape)
{
    out->shape = realloc(out->shape, len_shape * sizeof(*shape));
    out->stride = realloc(out->stride, len_shape * sizeof(*shape));
    out->len_shape = len_shape;
    out->num_entries = 1;
    for (int i = 0; i < len_shape; i++)
    {
        out->shape[i] = shape[i];
        out->stride[len_shape - i - 1] = out->num_entries;
        out->num_entries *= shape[len_shape - i - 1];
    }
    out->data = realloc(out->data, out->num_entries * sizeof(double));
}

void zeros(Tensor *out, unsigned *shape, int len_shape)
{
    shape_tensor(out, shape, len_shape);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 0;
    }
}

void ones(Tensor *out, unsigned *shape, int len_shape)
{
    shape_tensor(out, shape, len_shape);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 1;
    }
}

void rands(Tensor *out, unsigned *shape, int len_shape)
{
    srand(time(NULL));
    _random();
    shape_tensor(out, shape, len_shape);
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

void copy(Tensor *out, Tensor *tensor)
{
    shape_tensor(out, tensor->shape, tensor->len_shape);
    for (unsigned i = 0; i < tensor->num_entries; i++)
    {
        out->data[i] = tensor->data[_reorder_index(tensor, i, -1)];
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
    double new_entry;
    shape_tensor(out, shape, 2);
    for (unsigned i = 0; i < (unsigned)v1->len_shape; i++)
    {
        for (unsigned j = 0; j < (unsigned)v2->len_shape; j++)
        {
            shape[0] = i;
            shape[1] = j;
            new_entry = v1->data[_reorder_index(v1, i, 0)] * v2->data[_reorder_index(v2, j, 0)];
            set_element(out, shape, 2, new_entry);
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
            subtract = vector_1D->data[_reorder_index(vector_1D, i, 0)] * vector_1D->data[_reorder_index(vector_1D, j, 0)] / norm;
            out->data[(i + offset) * out->stride[0] + (j + offset)] -= subtract;
        }
    }
}

void pop(Tensor *tensor)
{
    free(tensor->data);
    free(tensor->shape);
    free(tensor->stride);
    free(tensor);
}
