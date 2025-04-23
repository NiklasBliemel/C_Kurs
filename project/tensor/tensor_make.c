#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

Tensor *emtpy_tensor(unsigned int *shape, int len_shape)
{
    Tensor *out = malloc(sizeof(*out));
    out->shape = malloc(len_shape * sizeof(*shape));
    out->stride = malloc(len_shape * sizeof(*shape));
    out->len_shape = len_shape;
    out->num_entries = 1;
    for (int i = 0; i < len_shape; i++)
    {
        out->shape[i] = shape[i];
        out->stride[len_shape - i - 1] = out->num_entries;
        out->num_entries *= shape[len_shape - i - 1];
    }
    out->data = malloc(out->num_entries * sizeof(double));
    return out;
}

Tensor *emtpy_like(Tensor *t)
{
    return emtpy_tensor(t->shape, t->len_shape);
}

Tensor *zeros(unsigned int *shape, int len_shape)
{
    Tensor *out = emtpy_tensor(shape, len_shape);
    for (unsigned int i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 0;
    }
    return out;
}

Tensor *zero_like(Tensor *t)
{
    return zeros(t->shape, t->len_shape);
}

Tensor *ones(unsigned int *shape, int len_shape)
{
    Tensor *out = emtpy_tensor(shape, len_shape);
    for (unsigned int i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 1;
    }
    return out;
}

Tensor *one_like(Tensor *t)
{
    return ones(t->shape, t->len_shape);
}

Tensor *arange(unsigned int range)
{
    unsigned int shape[] = {range};
    Tensor *out = emtpy_tensor(shape, 1);
    for (unsigned int i = 0; i < range; i++)
    {
        out->data[i] = i;
    }
    return out;
}

Tensor *linspace(double start, double end, unsigned int num_entries)
{
    unsigned int shape[] = {num_entries};
    Tensor *out = emtpy_tensor(shape, 1);
    for (unsigned int i = 0; i < num_entries; i++)
    {
        out->data[i] = start + (double)i / (num_entries - 1) * (end - start);
    }
    return out;
}

Tensor *copy(Tensor *tensor)
{
    Tensor *out = emtpy_like(tensor);
    for (unsigned int i = 0; i < tensor->num_entries; i++)
    {
        out->data[i] = tensor->data[_reorder_index(tensor, i, -1)];
    }
    return out;
}

Tensor *eye(unsigned int len_diag)
{
    unsigned int shape[2] = {len_diag, len_diag};
    Tensor *out = zeros(shape, 2);
    for (unsigned int i = 0; i < len_diag; i++)
    {
        shape[0] = i;
        shape[1] = i;
        set_element(out, shape, 2, 1);
    }
    return out;
}

Tensor *outer(Tensor *v1, Tensor *v2)
{
    unsigned int shape[2] = {v1->num_entries, v2->num_entries};
    double new_entry;
    Tensor *out = emtpy(shape, 2);
    for (unsigned int i = 0; i < v1->len_shape; i++)
    {
        for (unsigned int j = 0; j < v2->len_shape; j++)
        {
            shape[0] = i;
            shape[1] = j;
            new_entry = v1->data[_reorder_index(v1, i, 0)] * v2->data[_reorder_index(v2, j, 0)];
            set_element(out, shape, 2, new_entry);
        }
    }
    return out;
}

Tensor *house_holder(Tensor *vector_1D)
{
    Tensor *out = eye(vector_1D->num_entries);
    Tensor *temp = outer(vector_1D, vector_1D);
    double norm = dot(vector_1D, vector_1D) / 2;
    single_operation('/', temp, norm);
    operation('-', out, temp);
    pop(temp);
    return out;
}

void pop(Tensor *tensor)
{
    free(tensor->data);
    free(tensor->shape);
    free(tensor->stride);
    free(tensor);
}
