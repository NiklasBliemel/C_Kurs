#include <stdio.h>
#include "matrix.h"

int main()
{
    unsigned int shape[] = {4, 2, 2, 2, 2};
    Tensor *tensor = zeros(shape);
    print_shape(tensor);
    print_tensor(tensor);
    pop(tensor);
}