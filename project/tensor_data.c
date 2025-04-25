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
    fread(&t->num_entries, sizeof(unsigned), 1, file);
    t->data = realloc(t->data, t->num_entries * sizeof(double));
    fread(t->data, sizeof(double), t->num_entries, file);
    fread(&t->rank, sizeof(int), 1, file);
    t->shape = realloc(t->shape, t->rank * sizeof(unsigned));
    t->stride = realloc(t->stride, t->rank * sizeof(unsigned));
    fread(t->shape, sizeof(unsigned), t->rank, file);
    fread(t->stride, sizeof(unsigned), t->rank, file);
    fclose(file);
}