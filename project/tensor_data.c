#include <stdio.h>
#include <stdlib.h>
#include "tensor.h"

void save_tensor(Tensor *t, char *file_name)
{
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }
    // write every part of the Tensor structure one by one in binary (order is important for loading tensor)
    fwrite(&t->num_entries, sizeof(unsigned), 1, file);
    fwrite(t->data, sizeof(double), t->num_entries, file);
    fwrite(&t->rank, sizeof(int), 1, file);
    fwrite(t->shape, sizeof(unsigned), t->rank, file);
    fwrite(t->stride, sizeof(double), t->rank, file);
    fclose(file);
}

void load_tensor(Tensor *t, char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }
    // first read extracts num entries
    fread(&t->num_entries, sizeof(unsigned), 1, file);
    // use numentries to relocate enough memory and know how long to read data
    t->data = realloc(t->data, t->num_entries * sizeof(double));
    fread(t->data, sizeof(double), t->num_entries, file);
    // read rank, which is always stored after data
    fread(&t->rank, sizeof(int), 1, file);
    // relocate memory for shape and stride and continue reading (reading - length also defined by saved rank)
    t->shape = realloc(t->shape, t->rank * sizeof(unsigned));
    t->stride = realloc(t->stride, t->rank * sizeof(unsigned));
    fread(t->shape, sizeof(unsigned), t->rank, file);
    fread(t->stride, sizeof(unsigned), t->rank, file);
    fclose(file);
}