#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// initializes a new Tensor with 0 entries (allocates required memory) object and returns respective pointer
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

// force new shape onto tensor and reallocate memory accordingly (does nothing if tensor.shape is already as desired)
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

// creates Tensor object without allocating memory for tensor.data, already allocated memory form another tensor should be used instead
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

// force shapes tensor and fills with 0
void zeros(Tensor *out, unsigned *shape, int rank)
{
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 0;
    }
}

// force shapes tensor and fills with 1
void ones(Tensor *out, unsigned *shape, int rank)
{
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 1;
    }
}

// force shaped tensor and fills with double num
void fill(Tensor *out, unsigned *shape, int rank, double num)
{
    shape_tensor(out, shape, rank);
    for (unsigned i = 0; i < out->num_entries; i++)
    {
        out->data[i] = num;
    }
}

// force shapes tensor and fills with random double values in range (-1,1)
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

// force shapes tensor to 1-d array of size unsigned range and fills with acceding numbers (staring with 0)
void arange(Tensor *out, unsigned range)
{
    unsigned shape[] = {range};
    shape_tensor(out, shape, 1);
    for (unsigned i = 0; i < range; i++)
    {
        out->data[i] = i;
    }
}

// force shapes tensor to 1-d array of size unsigned num_entries and fills with from double start to double end
void linspace(Tensor *out, double start, double end, unsigned num_entries)
{
    unsigned shape[] = {num_entries};
    shape_tensor(out, shape, 1);
    for (unsigned i = 0; i < num_entries; i++)
    {
        out->data[i] = start + (double)i / (num_entries - 1) * (end - start);
    }
}

// force shapes Tensor *out to the same shape as Tensor *t and copies entries
void copy(Tensor *out, Tensor *t)
{
    shape_tensor(out, t->shape, t->rank);
    for (unsigned i = 0; i < t->num_entries; i++)
    {
        out->data[i] = t->data[_reorder(t, i)];
    }
}

// force shapes into 2-d square matrix of size len_diag x len_diag whereby each diagonal element is 1 and the rest are 0
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

// force shapes Tensor *out into 2-d v1.num_entries x v2.num_entries matrix converting it into the outerproduct of v1 and v2 (treats v1 and v2 as 1-d arrays)
void outer(Tensor *out, Tensor *v1, Tensor *v2)
{
    unsigned shape[2] = {v1->num_entries, v2->num_entries};
    shape_tensor(out, shape, 2);
    for (unsigned i = 0; i < (unsigned)v1->num_entries; i++)
    {
        for (unsigned j = 0; j < (unsigned)v2->num_entries; j++)
        {
            out->data[i * out->stride[1] + j * out->stride[0]] = v1->data[_reorder(v1, i)] * v2->data[_reorder(v2, j)];
        }
    }
}

// force shapes Tensor *out into size x size square matrix and converts it into the householder matrix with respect to Tensor *vector_1D
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

// delete tensor
void pop(Tensor *t)
{
    free(t->data);
    free(t->shape);
    free(t->stride);
    free(t);
}

// delete subspace tensor (does not free t.data because it is not explicitly allocated)
void pop_sub(Tensor *t)
{
    free(t->shape);
    free(t->stride);
    free(t);
}
