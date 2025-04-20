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

unsigned int _reorder(Tensor *tensor, unsigned int flat_index)
{
    unsigned int out = 0;
    if (flat_index >= tensor->num_entries)
    {
        printf("Flat index (%d) out of range (%d)\n", flat_index, tensor->num_entries);
        return out;
    }
    if (flat_index == tensor->num_entries - 1)
    {
        return tensor->num_entries - 1;
    }
    unsigned temp = tensor->num_entries;
    for (int i = 0; i < tensor->len_shape; i++)
    {
        temp /= tensor->shape[i];
        out += (flat_index / temp) * tensor->stride[i];
        flat_index = flat_index % temp;
    }
    return out % (tensor->num_entries - 1);
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

    for (int flat_index = 1; flat_index < tensor->num_entries - 1; flat_index++)
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

        printf("%4.3lf", tensor->data[_reorder(tensor, flat_index)]);
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
    if (flat_index != -1)
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
    if (flat_index != -1)
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
        if (shape[i] != -1)
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
    t->shape = realloc(t->shape, sizeof(int));
    *t->shape = t->num_entries;
    unsigned int temp = t->stride[t->len_shape - 1];
    t->stride = realloc(t->stride, sizeof(int));
    *t->stride = temp;
    t->len_shape = 1;
}

unsigned int *_copy_shape(Tensor *t)
{
    unsigned int *out = malloc(t->len_shape * sizeof(int));
    for (int i = 0; i < t->len_shape; i++)
    {
        out[i] = t->shape[i];
    }
    return out;
}

Tensor *copy(Tensor *tensor)
{
    Tensor *out = emtpy_tensor(tensor->shape, tensor->len_shape);
    unsigned int *shape = _copy_shape(tensor);
    int len_shape = tensor->len_shape;
    flat(tensor);
    for (unsigned int i = 0; i < tensor->num_entries - 1; i++)
    {
        out->data[i] = tensor->data[(i + *tensor->stride) % (tensor->num_entries - 1)];
    }
    out->data[tensor->num_entries - 1] = tensor->data[tensor->num_entries - 1];
    reshape(tensor, shape, len_shape);
    free(shape);
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
    unsigned int *shape1 = _copy_shape(t1);
    int len_shape1 = t1->len_shape;
    flat(t1);
    unsigned int *shape2 = _copy_shape(t2);
    int len_shape2 = t2->len_shape;
    flat(t2);

    if (op == '+')
    {
        for (int i = 0; i < out->num_entries; i++)
        {
            out->data[i] = t1->data[(i + *t1->stride) % (t1->num_entries - 1)] + t2->data[(i + *t1->stride) % (t1->num_entries - 1)];
        }
    }
    else if (op == '-')
    {
        for (int i = 0; i < out->num_entries; i++)
        {
            out->data[i] = t1->data[(i + *t1->stride) % (t1->num_entries - 1)] - t2->data[(i + *t1->stride) % (t1->num_entries - 1)];
        }
    }
    else if (op == '/')
    {
        for (int i = 0; i < out->num_entries; i++)
        {
            out->data[i] += t1->data[(i + *t1->stride) % (t1->num_entries - 1)] / t2->data[(i + *t1->stride) % (t1->num_entries - 1)];
        }
    }
    else if (op == '*')
    {
        for (int i = 0; i < out->num_entries; i++)
        {
            out->data[i] += t1->data[(i + *t1->stride) % (t1->num_entries - 1)] * t2->data[(i + *t1->stride) % (t1->num_entries - 1)];
        }
    }

    reshape(t1, shape1, len_shape1);
    reshape(t2, shape2, len_shape2);
    free(shape1);
    free(shape2);
    return out;
}

Tensor *add(Tensor *t1, Tensor *t2)
{
    return operation('+', t1, t2);
}

Tensor *dot(Tensor *t1, Tensor *t2)
{
    Tensor *out = emtpy_like(t1);
    if (t1->len_shape < 2 || t2->len_shape < 2)
    {
        printf("Dimensions smaller than 2!\n");
        return out;
    }
    if (t1->shape[t1->len_shape - 1] != t2->shape[t2->len_shape - 2])
    {
        printf("ERR1: Shapes don't match!\n");
        return out;
    }
    if (t1->len_shape == t2->len_shape)
    {
        for (int i = 0; i < t1->len_shape - 2; i++)
        {
            if (t1->shape[i] != t2->shape[i])
            {
                printf("ERR2: Shapes don't match!\n");
                return out;
            }
        }
        unsigned int *shape1 = _copy_shape(t1);
        int len_shape1 = t1->len_shape;
        unsigned int *shape2 = _copy_shape(t2);
        int len_shape2 = t2->len_shape;
        unsigned int new_shape1[3] = {-1};
        unsigned int new_shape2[3] = {-1};
        for (int i = 1; i < 3; i++)
        {
            new_shape1[i] = shape1[len_shape1 - 3 + i];
            new_shape2[i] = shape2[len_shape1 - 3 + i];
        }
        reshape(t1, new_shape1, 3);
        reshape(t2, new_shape2, 3);

        // insert dot product

        reshape(t1, shape1, len_shape1);
        free(shape1);
        reshape(t2, shape2, len_shape2);
        free(shape2);
    }
}
