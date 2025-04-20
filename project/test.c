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

    unsigned int new_shape[2] = {10, 4};
    reshape(tensor, new_shape, 2);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);
    printf("\n");

    int swap_list[2] = {1, 0};
    permute(tensor, swap_list, 2);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);
    printf("\n");

    unsigned int new_new_shape[3] = {5, 4, -1};
    reshape(tensor, new_new_shape, 3);
    print_stride(tensor);
    print_shape(tensor);
    print_tensor(tensor);

    Tensor *copy_tensor = copy(tensor);
    pop(tensor);
    print_stride(copy_tensor);
    print_shape(copy_tensor);
    print_tensor(copy_tensor);

    unsigned int newshape[] = {40};
    reshape(copy_tensor, newshape, 1);
    print_stride(copy_tensor);
    print_shape(copy_tensor);
    print_tensor(copy_tensor);
}