#include "tensor.h"
#include <stdio.h>
#include <stdlib.h>

// QR decomposition for 2-d square matrix
void _QR(Tensor *Q, Tensor *R, Tensor *A)
{
    // setup
    unsigned size = A->shape[0];
    Tensor *Qn = init_tensor();
    Tensor *u = init_tensor();
    Tensor *Q_temp = init_tensor();
    Tensor *R_temp = init_tensor();

    // inital step
    extract_col(u, A, 0, 0);
    u->data[0] -= norm(u);
    house_holder(Q, u, size);
    matmul(R, Q, A);

    // next steps
    for (unsigned i = 1; i < size - 1; i++)
    {
        extract_col(u, R, i, i);
        u->data[0] -= norm(u);
        house_holder(Qn, u, size);
        matmul(R_temp, Qn, R);
        matmul(Q_temp, Q, Qn);
        copy(Q, Q_temp);
        copy(R, R_temp);
    }
    pop(Qn);
    pop(u);
    pop(Q_temp);
    pop(R_temp);
}

// QR decomposition for every 2-d square matrix in the last two dimensions
void QR(Tensor *Q, Tensor *R, Tensor *A)
{
    if (A->shape[A->rank - 1] != A->shape[A->rank - 2])
    {
        printf("Matrix has to be square!\n");
        return;
    }
    if (A->shape[A->rank - 1] < 2)
    {
        printf("Matrix at least rank 2!\n");
        return;
    }
    if (A == Q || A == R || Q == R)
    {
        printf("Locations of Q,R and A has to be different!\n");
        return;
    }

    if (A->rank == 2)
    {
        _QR(Q, R, A);
        return;
    }

    // calculate how many matrices to decompose
    unsigned residual = A->num_entries;
    residual /= A->shape[A->rank - 1];
    residual /= A->shape[A->rank - 2];

    // force shape Q and R into shape of A
    shape_tensor(Q, A->shape, A->rank);
    shape_tensor(R, A->shape, A->rank);

    // create 2-d subspace tensor for every matrix and decompose every matrix one by one
    unsigned shape[2] = {A->shape[A->rank - 2], A->shape[A->rank - 1]};
    Tensor *a = subspace(shape, 2, A->data);
    Tensor *q = subspace(shape, 2, Q->data);
    Tensor *r = subspace(shape, 2, R->data);
    for (unsigned i = 0; i < residual; i++)
    {
        a->data = &A->data[i * A->stride[A->rank - 3] % (A->num_entries - 1)];
        q->data = &Q->data[i * Q->stride[A->rank - 3] % (Q->num_entries - 1)];
        r->data = &R->data[i * R->stride[A->rank - 3] % (R->num_entries - 1)];
        _QR(q, r, a);
    }

    // delete created subspaces
    pop_sub(a);
    pop_sub(q);
    pop_sub(r);
}
