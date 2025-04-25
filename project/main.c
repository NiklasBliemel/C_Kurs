#include <stdio.h>
#include <stdlib.h>
#include "tensor.h"

int main()
{
    Tensor *A = init_tensor();
    Tensor *Q = init_tensor();
    Tensor *R = init_tensor();
    Tensor *A_test = init_tensor();
    Tensor *Q_t = init_tensor();
    Tensor *Q_test = init_tensor();

    unsigned shape[3] = {2, 3, 3};
    rands(A, shape, 3);

    QR(Q, R, A);

    // Test if QR = A
    matmul(A_test, Q, R);

    // Test if Q Q_t = 1
    copy(Q_t, Q);
    transpose(Q_t);
    matmul(Q_test, Q, Q_t);

    printf("A:\n");
    print_tensor(A);
    printf("\nQ:\n");
    print_tensor(Q);
    printf("\nR:\n");
    print_tensor(R);
    printf("\nQR:\n");
    print_tensor(A_test);
    printf("\nQQ_t:\n");
    print_tensor(Q_test);
    printf("\n");

    pop(A);
    pop(Q);
    pop(R);
    pop(A_test);
    pop(Q_t);
    pop(Q_test);
}