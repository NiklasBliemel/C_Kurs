#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double get_element(Tensor *t, unsigned *index_list, int len_list)
{
    unsigned flat_index = _flatten_index(t, index_list, len_list);
    if (flat_index != (unsigned)-1)
    {
        return t->data[flat_index];
    }
    else
    {
        return -1;
    }
}

double set_element(Tensor *t, unsigned *index_list, int len_list, double new_entry)
{
    unsigned flat_index = _flatten_index(t, index_list, len_list);
    if (flat_index != (unsigned)-1)
    {
        double out = t->data[flat_index];
        t->data[flat_index] = new_entry;
        return out;
    }
    else
    {
        return -1;
    }
}

void extract_col(Tensor *out, Tensor *t, unsigned offset, int col)
{
    if (t->rank < 2)
    {
        printf("Rank at least 2!\n");
        return;
    }

    unsigned shape[t->rank - 1];
    for (int i = 0; i < t->rank - 2; i++)
    {
        shape[i] = t->shape[i];
    }
    shape[t->rank - 2] = t->shape[t->rank - 2] - offset;
    shape_tensor(out, shape, t->rank - 1);

    unsigned residual = out->num_entries / out->shape[out->rank - 1];
    unsigned index_1;
    unsigned index_2;
    for (unsigned i = 0; i < residual; i++)
    {
        for (unsigned x = 0; x < out->shape[out->rank - 1]; x++)
        {
            index_1 = i * out->stride[out->rank - 2] + x;
            index_2 = i * t->stride[t->rank - 3] + (x + offset) * t->stride[t->rank - 2] + col * t->stride[t->rank - 1];
            out->data[index_1] = t->data[index_2];
        }
    }
}

double dot(Tensor *v1, Tensor *v2)
{
    if (v1->rank != 1 || v2->rank != 1)
    {
        printf("Rank should be 1!\n");
        return -1;
    }
    if (v1->num_entries != v2->num_entries)
    {
        printf("Sizes don't match!\n");
        return -1;
    }
    double out = 0;
    for (unsigned i = 0; i < v1->num_entries; i++)
    {
        out += v1->data[i] * v2->data[i];
    }
    return out;
}

void single_operation(char op, Tensor *t, double num)
{
    if (op == '+')
    {
        for (unsigned i = 0; i < t->num_entries; i++)
        {
            t->data[i] += num;
        }
    }
    else if (op == '-')
    {
        for (unsigned i = 0; i < t->num_entries; i++)
        {
            t->data[i] -= num;
        }
    }
    else if (op == '/')
    {
        if (num == 0)
        {
            printf("Zero Division Error!\n");
            return;
        }
        for (unsigned i = 0; i < t->num_entries; i++)
        {
            t->data[i] /= num;
        }
    }
    else if (op == '*')
    {
        for (unsigned i = 0; i < t->num_entries; i++)
        {
            t->data[i] *= num;
        }
    }
}

void operation(Tensor *out, char op, Tensor *t1, Tensor *t2)
{
    if (out != t1)
    {
        shape_tensor(out, t1->shape, t1->rank);
    }
    if (t1->rank != t2->rank)
    {
        printf("Shapes don't match\n");
        return;
    }
    for (int i = 0; i < t1->rank; i++)
    {
        if (t1->shape[i] != t2->shape[i])
        {
            printf("Shapes don't match\n");
            return;
        }
    }

    if (op == '+')
    {
        for (unsigned i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] = t1->data[_reorder(t1, i)] + t2->data[_reorder(t2, i)];
        }
    }
    else if (op == '-')
    {
        for (unsigned i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] = t1->data[_reorder(t1, i)] - t2->data[_reorder(t2, i)];
        }
    }
    else if (op == '/')
    {
        for (unsigned i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] += t1->data[_reorder(t1, i)] / t2->data[_reorder(t2, i)];
        }
    }
    else if (op == '*')
    {
        for (unsigned i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] += t1->data[_reorder(t1, i)] * t2->data[_reorder(t2, i)];
        }
    }
}

void matmul(Tensor *out, Tensor *t1, Tensor *t2)
{
    // basic error catches
    if (t1->rank < 2 || t2->rank < 2)
    {
        printf("Dimensions smaller than 2!\n");
        return;
    }
    if (t1->shape[t1->rank - 1] != t2->shape[t2->rank - 2])
    {
        printf("Shapes don't match!\n");
        return;
    }
    if (out == t1 || out == t2)
    {
        printf("Target tensor can't be used for matmul!\n");
        return;
    }

    // setup target tensor
    unsigned *out_shape;
    int rank;
    if (t1->rank <= t2->rank)
    {
        rank = t2->rank;
        out_shape = malloc(t2->rank * sizeof(unsigned));
        for (int i = 0; i < t2->rank - 2; i++)
        {
            out_shape[i] = t2->shape[i];
        }
    }
    else
    {
        rank = t1->rank;
        out_shape = malloc(t1->rank * sizeof(int));
        for (int i = 0; i < t1->rank - 2; i++)
        {
            out_shape[i] = t1->shape[i];
        }
    }
    out_shape[rank - 2] = t1->shape[t1->rank - 2];
    out_shape[rank - 1] = t2->shape[t2->rank - 1];
    zeros(out, out_shape, rank);
    free(out_shape);

    for (int i = 3; i <= out->rank; i++)
    {
        if (t1->rank >= i && t2->rank >= i)
        {
            if (t1->shape[t1->rank - i] != t2->shape[t2->rank - i])
            {
                printf("Shapes don't match!\n");
                return;
            }
        }
    }

    unsigned residual = out->num_entries;
    unsigned residual_1 = t1->num_entries;
    unsigned residual_2 = t2->num_entries;
    for (int i = 1; i <= 2; i++)
    {
        residual /= out->shape[out->rank - i];
        residual_1 /= t1->shape[t1->rank - i];
        residual_2 /= t2->shape[t2->rank - i];
    }

    for (unsigned i = 0; i < residual; i++)
    {
        for (unsigned x = 0; x < out->shape[out->rank - 2]; x++)
        {
            for (unsigned y = 0; y < out->shape[out->rank - 1]; y++)
            {
                for (unsigned k = 0; k < t1->shape[t1->rank - 1]; k++)
                {
                    out->data[_reorder_three(out, residual, i, x, y)] +=
                        t1->data[_reorder_three(t1, residual_1, i, x, k)] * t2->data[_reorder_three(t2, residual_2, i, k, y)];
                }
            }
        }
    }
}

double norm(Tensor *t)
{
    return sqrt(dot(t, t));
}
