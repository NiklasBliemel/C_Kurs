#include <stdlib.h>
#include <stdio.h>
#include "tensor.h"

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

void pop(Tensor *tensor)
{
    free(tensor->data);
    free(tensor->shape);
    free(tensor->stride);
    free(tensor);
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

void print_tensor(Tensor *tensor)
{
    int extra_print;
    unsigned int temp;

    for (int i = 0; i < tensor->len_shape; i++)
    {
        printf("[");
    }
    printf("%4.3lf", *tensor->data);

    for (unsigned int flat_index = 1; flat_index < tensor->num_entries - 1; flat_index++)
    {
        extra_print = 0;
        temp = 1;
        for (int i = 0; i < tensor->len_shape; i++)
        {
            temp *= tensor->shape[tensor->len_shape - 1 - i];
            if (flat_index % temp == 0)
            {
                extra_print++;
            }
        }
        if (extra_print == 0)
        {
            printf(" ,");
        }
        else
        {
            for (int i = 0; i < extra_print; i++)
            {
                printf("]");
            }
            printf(",");
            for (int i = 0; i < extra_print; i++)
            {
                printf("\n");
            }
            for (int i = 0; i < tensor->len_shape; i++)
            {
                if (i < tensor->len_shape - extra_print)
                {
                    printf(" ");
                }
                else
                {
                    printf("[");
                }
            }
        }

        printf("%4.3lf", tensor->data[_reorder_index(tensor, flat_index, -1)]);
    }

    printf(", %4.3lf", tensor->data[tensor->num_entries - 1]);
    for (int i = 0; i < tensor->len_shape; i++)
    {
        printf("]");
    }
    printf("\n");
}

void print_shape(Tensor *tensor)
{
    printf("Shape: [%d", *tensor->shape);
    for (int i = 1; i < tensor->len_shape; i++)
    {
        printf(", %d", tensor->shape[i]);
    }
    printf("]\n");
}

void print_stride(Tensor *tensor)
{
    printf("Stride: [%d", *tensor->stride);
    for (int i = 1; i < tensor->len_shape; i++)
    {
        printf(", %d", tensor->stride[i]);
    }
    printf("]\n");
}

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

Tensor *copy(Tensor *tensor)
{
    Tensor *out = emtpy_like(tensor);
    for (unsigned int i = 0; i < tensor->num_entries; i++)
    {
        out->data[i] = tensor->data[_reorder_index(tensor, i, -1)];
    }
    return out;
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
