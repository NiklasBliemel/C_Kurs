#ifndef TENSOR_H_
#define TENSOR_H_

typedef struct
{
    unsigned num_entries;
    int len_shape;
    unsigned *shape;
    unsigned *stride;
    double *data;
} Tensor;

// --------------------------------------------- helper functions

unsigned _flatten_index(Tensor *tensor, unsigned *index_list, int len_list);

unsigned _reorder_index(Tensor *tensor, unsigned index, int dim);

unsigned _reorder_three(Tensor *tensor, unsigned residual, unsigned i1, unsigned i2, unsigned i3);

double _random();

// --------------------------------------------- initialisation functions

Tensor *init_tensor();

void shape_tensor(Tensor *out, unsigned *shape, int len_shape);

void zeros(Tensor *out, unsigned *shape, int len_shape);

void ones(Tensor *out, unsigned *shape, int len_shape);

void rands(Tensor *out, unsigned *shape, int len_shape);

void arange(Tensor *out, unsigned range);

void linspace(Tensor *out, double start, double end, unsigned num_entries);

void copy(Tensor *out, Tensor *tensor);

void eye(Tensor *out, unsigned len_diag);

void outer(Tensor *out, Tensor *v1, Tensor *v2);

void house_holder(Tensor *out, Tensor *vector_1D, unsigned size);

void pop(Tensor *tensor);

// --------------------------------------------- reshape functions

void reshape(Tensor *tensor, unsigned *shape, int len_shape);

void permute(Tensor *tensor, int *permutation, int len_permutation);

void flat(Tensor *t);

// --------------------------------------------- base operations

double get_element(Tensor *tensor, unsigned *index_list, int len_list);

double set_element(Tensor *tensor, unsigned *index_list, int len_list, double new_entry);

void extract_col(Tensor *out, Tensor *t, unsigned offset, int col);

double dot(Tensor *v1, Tensor *v2);

void single_operation(char op, Tensor *t1, double num);

void operation(Tensor *out, char op, Tensor *t1, Tensor *t2);

void matmul(Tensor *out, Tensor *t1, Tensor *t2);

double norm(Tensor *t);

// --------------------------------------------- algorithms

// --------------------------------------------- print functions

void print_tensor(Tensor *tensor);

void print_shape(Tensor *tensor);

void print_stride(Tensor *tensor);

#endif // TENSOR_H_