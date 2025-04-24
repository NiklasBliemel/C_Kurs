#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

void QR(Tensor *Q, Tensor *R, Tensor *A)
{
    if (A->len_shape != 2)
    {
        printf("Tensor has to be 2 dimensional!\n");
        return;
    }
    if (A->shape[0] != A->shape[1])
    {
        printf("Matrix has to be square!\n");
        return;
    }
    if (A->shape[0] < 2)
    {
        printf("Matrix at least size 2!\n");
        return;
    }

    unsigned size = A->shape[0];
    Tensor *Qn = init_tensor();
    Tensor *u = init_tensor();

    extract_col(u, A, 0, 0);
    u->data[0] -= norm(u);
    house_holder(Qn, u, size);
    copy(Q, Qn);
    matmul(R, Qn, A);
    for (unsigned i = 1; i < size - 1; i++)
    {
        extract_col(u, R, i, i);
        u->data[0] -= norm(u);
        house_holder(Qn, u, size);
        matmul(Q, Q, Qn);
        matmul(R, Qn, R);
    }
}
