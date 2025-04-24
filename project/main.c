#include <stdio.h>
#include <stdlib.h>
#include "tensor.h"

int main()
{
    Tensor *A = init_tensor();
    Tensor *Q = init_tensor();
    Tensor *R = init_tensor();
    Tensor *A_test = init_tensor();
    Tensor *Q_test = init_tensor();
    unsigned size = 3;

    unsigned shape[] = {size, size};
    int transpose[] = {1, 0};
    rands(A, shape, 2);

    QR(Q, R, A);
    matmul(A_test, Q, R);

    copy(Q_test, Q);
    permute(Q_test, transpose, 2);
    matmul(Q_test, Q, Q_test);

    printf("\nA:\n");
    print_tensor(A);
    printf("\nQ:\n");
    print_tensor(Q);
    printf("\nR:\n");
    print_tensor(R);
    printf("\nQR\n");
    print_tensor(A_test);
    printf("\nQ Q.T\n");
    print_tensor(Q_test);
}