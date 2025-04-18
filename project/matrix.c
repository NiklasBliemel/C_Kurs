#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

unsigned int *zero_list(int len)
{
    unsigned int *out = malloc(len + 1);
    *out = len;
    for (int i = 1; i <= len; i++)
    {
        out[i] = 0;
    }
    return out;
}

void _print_list(unsigned int *list)
{
    printf("[");
    for (int i = 1; i < *list; i++)
    {
        printf("%d, ", list[i]);
    }
    printf("%d]\n", list[*list]);
}

Tensor *emtpy_tensor(unsigned int *shape)
{
    Tensor *out = malloc(sizeof(*out));
    out->index_factors = malloc((*shape + 1) * sizeof(*shape));
    out->shape = malloc((*shape + 1) * sizeof(*shape));
    int num_entries = 1;
    *out->shape = *shape;
    *out->index_factors = *shape;
    for (int i = 1; i <= *shape; i++)
    {
        out->shape[i] = shape[i];
        out->index_factors[i] = num_entries;
        num_entries *= shape[i];
    }
    out->data = malloc(num_entries * sizeof(double));
    out->num_entries = num_entries;
    return out;
}

void pop(Tensor *tensor)
{
    free(tensor->data);
    free(tensor->index_factors);
    free(tensor->shape);
    free(tensor);
}

Tensor *zeros(unsigned int *shape)
{
    Tensor *out = emtpy_tensor(shape);
    for (unsigned int i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 0;
    }
    return out;
}

Tensor *zeros_like(Tensor *tensor)
{
    return zeros(tensor->shape);
}

Tensor *ones(unsigned int *shape)
{
    Tensor *out = emtpy_tensor(shape);
    for (unsigned int i = 0; i < out->num_entries; i++)
    {
        out->data[i] = 1;
    }
    return out;
}

Tensor *ones_like(Tensor *tensor)
{
    return ones(tensor->shape);
}

Tensor *tensor(unsigned int *shape, double *data)
{
    Tensor *out = emtpy_tensor(shape);
    for (unsigned int i = 0; i < out->num_entries; i++)
    {
        out->data[i] = data[i];
    }
    return out;
}

unsigned long int _flatten_index(Tensor *tensor, unsigned int *index_list)
{
    if (*tensor->shape != *index_list)
    {
        printf("Index List (%d) does not match Tensor shape (%d)\n", *index_list, *tensor->shape);
        return -1;
    }
    unsigned long int index = 0;
    for (int i = 1; i <= *tensor->shape; i++)
    {
        if (tensor->shape[i] < index_list[i])
        {
            printf("%d. Index (%d) is out of range (%d)\n", i, index_list[i], tensor->shape[i]);
            index = -1;
        }
        else if (index != -1)
        {
            index += index_list[i] * tensor->index_factors[i];
        }
    }
    return index;
}

void print_tensor(Tensor *tensor)
{
    int flat_index = 0;
    int index_factor;
    for (int i = 1; i < *tensor->shape; i++)
    {
        printf("[");
    }
    printf("[%4.3lf", *tensor->data);

    for (int flat_index = 1; flat_index < tensor->num_entries - 1; flat_index++)
    {
        index_factor = 1;
        for (int i = *tensor->shape; i > 0; i--)
        {
            index_factor *= tensor->shape[i];
            if (flat_index % index_factor == 0)
            {
                printf("]");
            }
            else if (i < *tensor->shape)
            {
                {
                    printf(",");
                }
                for (int j = *tensor->shape; j > i; j--)
                {
                    printf("\n");
                }
                if (i != *tensor->shape)
                {
                    for (int j = 0; j < *tensor->shape; j++)
                    {
                        if (j < i && i)
                        {
                            printf(" ");
                        }
                        else
                        {
                            printf("[");
                        }
                    }
                }
                break;
            }
            else
            {
                printf(", ");
                break;
            }
        }

        printf("%4.3lf", tensor->data[flat_index]);
    }

    printf(", %4.3lf]", tensor->data[tensor->num_entries]);
    for (int i = 1; i < *tensor->shape; i++)
    {
        printf("]");
    }
    printf("\n");
}

void print_shape(Tensor *tensor)
{
    printf("Shape:\t");
    _print_list(tensor->shape);
}

double get_element(Tensor *tensor, unsigned int *index_list)
{
    unsigned int flat_index = _flatten_index(tensor, index_list);
    if (flat_index != -1)
    {
        return tensor->data[flat_index];
    }
    else
    {
        return -1;
    }
}

double set_element(Tensor *tensor, unsigned int *index_list, double new_entry)
{
    unsigned int flat_index = _flatten_index(tensor, index_list);
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
