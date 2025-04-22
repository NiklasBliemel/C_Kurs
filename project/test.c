#include <stdio.h>
#include <stdlib.h>
#include "tensor.h"

int main()
{
    Tensor *tensor = arange(40);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);
    printf("\n");

    printf("Reshape 1 Test:\n\n");
    unsigned int new_shape[2] = {10, 4};
    reshape(tensor, new_shape, 2);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);
    printf("\n");

    printf("Permute Test:\n\n");
    int swap_list[2] = {1, 0};
    permute(tensor, swap_list, 2);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);
    printf("\n");

    printf("Reshape 2 Test:\n\n");
    unsigned int new_new_shape[3] = {5, 4, -1};
    reshape(tensor, new_new_shape, 3);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);
    printf("\n");

    printf("Copy Test:\n\n");
    Tensor *copy_tensor = copy(tensor);
    Tensor *add_tensor = operation('+', tensor, copy_tensor);
    pop(tensor);
    pop(copy_tensor);
    print_stride(add_tensor);
    print_shape(add_tensor);
    print_tensor(add_tensor);
    printf("\n");

    printf("Matmul Test:\n\n");
    unsigned int s1[] = {2, 2, 3};
    unsigned int s2[] = {2, 2, 3, 2};
    Tensor *t1 = arange(2 * 2 * 3);
    reshape(t1, s1, 3);
    Tensor *t2 = arange(2 * 2 * 3 * 2);
    reshape(t2, s2, 4);
    print_tensor(t1);
    printf("\n");
    print_tensor(t2);
    printf("\n");
    Tensor *dot_tensor = matmul(t1, t2);
    print_tensor(dot_tensor);
    printf("\n");

    printf("Flat Test\n");
    flat(dot_tensor);
    print_tensor(dot_tensor);
}