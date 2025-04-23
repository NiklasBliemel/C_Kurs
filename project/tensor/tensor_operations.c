#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

double get_element(Tensor *tensor, unsigned int *index_list, int len_list)
{
    unsigned int flat_index = _flatten_index(tensor, index_list, len_list);
    if (flat_index != (unsigned int)-1)
    {
        return tensor->data[flat_index];
    }
    else
    {
        return -1;
    }
}

double set_element(Tensor *tensor, unsigned int *index_list, int len_list, double new_entry)
{
    unsigned int flat_index = _flatten_index(tensor, index_list, len_list);
    if (flat_index != (unsigned int)-1)
    {
        double out = tensor->data[flat_index];
        tensor->data[flat_index] = new_entry;
        return out;
    }
    else
    {
        return -1;
    }
}

double dot(Tensor *v1, Tensor *v2)
{
    double out = 0;
    if (v1->num_entries == v2->num_entries && v1->len_shape == 1 && v2->len_shape == 1)
    {
        for (unsigned int i = 0; i < v1->num_entries; i++)
        {
            out += v1->data[_reorder_index(v1, i, -1)] * v2->data[_reorder_index(v2, i, -1)];
        }
        return out;
    }
    printf("Error 4!\n");
    return -1;
}

void single_operation(char op, Tensor *t, double num)
{
    if (op == '+')
    {
        for (unsigned int i = 0; i < t->num_entries; i++)
        {
            t->data[i] += num;
        }
    }
    else if (op == '-')
    {
        for (unsigned int i = 0; i < t->num_entries; i++)
        {
            t->data[i] -= num;
        }
    }
    else if (op == '/')
    {
        if (num == 0)
        {
            printf("Zero Devision Error!\n");
            return;
        }
        for (unsigned int i = 0; i < t->num_entries; i++)
        {
            t->data[i] /= num;
        }
    }
    else if (op == '*')
    {
        for (unsigned int i = 0; i < t->num_entries; i++)
        {
            t->data[i] *= num;
        }
    }
}

Tensor *operation(char op, Tensor *t1, Tensor *t2)
{
    Tensor *out = emtpy_like(t1);
    if (t1->len_shape != t2->len_shape)
    {
        printf("Shapes don't match\n");
        return out;
    }
    for (int i = 0; i < t1->len_shape; i++)
    {
        if (t1->shape[i] != t2->shape[i])
        {
            printf("Shapes don't match\n");
            return out;
        }
    }

    if (op == '+')
    {
        for (unsigned int i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] = t1->data[_reorder_index(t1, i, -1)] + t2->data[_reorder_index(t2, i, -1)];
        }
    }
    else if (op == '-')
    {
        for (unsigned int i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] = t1->data[_reorder_index(t1, i, -1)] - t2->data[_reorder_index(t2, i, -1)];
        }
    }
    else if (op == '/')
    {
        for (unsigned int i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] += t1->data[_reorder_index(t1, i, -1)] / t2->data[_reorder_index(t2, i, -1)];
        }
    }
    else if (op == '*')
    {
        for (unsigned int i = 0; i < out->num_entries - 1; i++)
        {
            out->data[i] += t1->data[_reorder_index(t1, i, -1)] * t2->data[_reorder_index(t2, i, -1)];
        }
    }
    return out;
}

Tensor *matmul(Tensor *t1, Tensor *t2)
{
    if (t1->len_shape < 2 || t2->len_shape < 2)
    {
        printf("Dimensions smaller than 2!\n");
        return zeros(t1->shape, t1->len_shape);
    }
    unsigned int *out_shape;
    int len_shape;
    Tensor *out;
    if (t1->len_shape <= t2->len_shape)
    {
        out_shape = malloc(t2->len_shape * sizeof(int));
        for (int i = 0; i < t2->len_shape - 2; i++)
        {
            out_shape[i] = t2->shape[i];
        }
        len_shape = t2->len_shape;
    }
    else
    {
        out_shape = malloc(t1->len_shape * sizeof(int));
        for (int i = 0; i < t1->len_shape - 2; i++)
        {
            out_shape[i] = t1->shape[i];
        }
        len_shape = t1->len_shape;
    }
    out_shape[t2->len_shape - 2] = t1->shape[t1->len_shape - 2];
    out_shape[t2->len_shape - 1] = t2->shape[t2->len_shape - 1];
    out = zeros(out_shape, len_shape);

    if (t1->shape[t1->len_shape - 1] != t2->shape[t2->len_shape - 2])
    {
        printf("ERR1: Shapes don't match!\n");
        return out;
    }

    for (int i = 3; i <= out->len_shape; i++)
    {
        if (t1->len_shape >= i && t2->len_shape >= i)
        {
            if (t1->shape[t1->len_shape - i] != t2->shape[t2->len_shape - i])
            {
                printf("ERR2: Shapes don't match!\n");
                return out;
            }
        }
    }

    unsigned int residual = out->num_entries;
    unsigned int residual_1 = t1->num_entries;
    unsigned int residual_2 = t2->num_entries;
    for (int i = 1; i <= 2; i++)
    {
        residual /= out->shape[out->len_shape - i];
        residual_1 /= t1->shape[t1->len_shape - i];
        residual_2 /= t2->shape[t2->len_shape - i];
    }

    for (unsigned int i = 0; i < residual; i++)
    {
        for (unsigned int x = 0; x < out->shape[out->len_shape - 2]; x++)
        {
            for (unsigned int y = 0; y < out->shape[out->len_shape - 1]; y++)
            {
                for (unsigned int k = 0; k < t1->shape[t1->len_shape - 1]; k++)
                {
                    out->data[_reorder_three(out, residual, i, x, y)] +=
                        t1->data[_reorder_three(t1, residual_1, i, x, k)] * t2->data[_reorder_three(t2, residual_2, i, k, y)];
                }
            }
        }
    }
    return out;
}
