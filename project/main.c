#include <stdio.h>
#include <stdlib.h>
#include "tensor.h"

int main()
{
    Tensor *t = init_tensor();
    Tensor *t2 = init_tensor();
    Tensor *p = init_tensor();
    Tensor *vector = init_tensor();

    unsigned shape[] = {3, 3};
    zeros(t, shape, 2);
    for (unsigned i = 0; i < t->num_entries; i++)
    {
        t->data[i] = i + 1;
    }

    extract_col(vector, t, 0, 0);
    vector->data[0] -= norm(vector);
    house_holder(p, vector, 3);
    print_tensor(vector);
    printf("\n");
    print_tensor(p);
    printf("\n");
    print_tensor(t);
    printf("\n");
    matmul(t2, p, t);
    print_tensor(t2);
}