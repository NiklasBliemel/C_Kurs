#ifndef TENSOR_H_
#define TENSOR_H_

typedef struct
{
    unsigned int num_entries;
    int len_shape;
    unsigned int *shape;
    unsigned int *stride;
    double *data;
} Tensor;

// --------------------------------------------- helper functions

unsigned int _flatten_index(Tensor *tensor, unsigned int *index_list, int len_list);

unsigned int _reorder_index(Tensor *tensor, unsigned int index, int dim);

unsigned int _reorder_three(Tensor *tensor, unsigned int residual, unsigned int i1, unsigned int i2, unsigned int i3);

double abs(double num);

// --------------------------------------------- initialisation functions

Tensor *emtpy_tensor(unsigned int *shape, int len_shape);

Tensor *emtpy_like(Tensor *t);

Tensor *zeros(unsigned int *shape, int len_shape);

Tensor *zero_like(Tensor *t);

Tensor *ones(unsigned int *shape, int len_shape);

Tensor *one_like(Tensor *t);

Tensor *arange(unsigned int range);

Tensor *linspace(double start, double end, unsigned int num_entries);

Tensor *copy(Tensor *tensor);

Tensor *eye(unsigned int len_diag);

Tensor *outer(Tensor *v1, Tensor *v2);

Tensor *house_holder(Tensor *vector_1D);

void pop(Tensor *tensor);

// --------------------------------------------- reshape functions

void reshape(Tensor *tensor, unsigned int *shape, int len_shape);

void permute(Tensor *tensor, int *permutation, int len_permutation);

void flat(Tensor *t);

// --------------------------------------------- base operations

double get_element(Tensor *tensor, unsigned int *index_list, int len_list);

double set_element(Tensor *tensor, unsigned int *index_list, int len_list, double new_entry);

double dot(Tensor *v1, Tensor *v2);

void single_operation(char op, Tensor *t1, double num);

Tensor *operation(char op, Tensor *t1, Tensor *t2);

Tensor *matmul(Tensor *t1, Tensor *t2);

// --------------------------------------------- algorithms

// --------------------------------------------- print functions

void print_tensor(Tensor *tensor);

void print_shape(Tensor *tensor);

void print_stride(Tensor *tensor);

#endif // TENSOR_H_