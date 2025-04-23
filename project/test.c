#include <stdio.h>
#include <stdlib.h>
#include "tensor/tensor.h"

int main()
{
    Tensor *tensor = linspace(0, 10, 100);
    print_tensor(tensor);
}