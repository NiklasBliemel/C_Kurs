#ifndef TENSOR_H_
#define TENSOR_H_

typedef struct
{
    unsigned num_entries;
    int rank;
    unsigned *shape;
    unsigned *stride;
    double *data;
} Tensor;

// --------------------------------------------- helper functions

unsigned _fill_shape(Tensor *t, unsigned *shape, int rank);

unsigned _flatten_index(Tensor *t, unsigned *index_list, int len_list);

unsigned _reorder(Tensor *t, unsigned flat_index);

unsigned _reorder_three(Tensor *t, unsigned residual, unsigned x, unsigned y, unsigned z);

double _random();

// --------------------------------------------- initialisation functions

Tensor *init_tensor();

void shape_tensor(Tensor *out, unsigned *shape, int rank);

Tensor *subspace(unsigned *shape, int rank, double *datapoint);

void zeros(Tensor *out, unsigned *shape, int rank);

void ones(Tensor *out, unsigned *shape, int rank);

void fill(Tensor *out, unsigned *shape, int rank, double num);

void rands(Tensor *out, unsigned *shape, int rank);

void arange(Tensor *out, unsigned range);

void linspace(Tensor *out, double start, double end, unsigned num_entries);

void copy(Tensor *out, Tensor *t);

void eye(Tensor *out, unsigned len_diag);

void outer(Tensor *out, Tensor *v1, Tensor *v2);

void house_holder(Tensor *out, Tensor *vector_1D, unsigned size);

void pop(Tensor *t);

void pop_sub(Tensor *t);

// --------------------------------------------- reshape functions

void reshape(Tensor *t, unsigned *shape, int rank);

void view(Tensor *t, unsigned *shape, int rank);

void permute(Tensor *t, int *permutation, int len_permutation);

void flat(Tensor *t);

void transpose(Tensor *t);

// --------------------------------------------- base operations

double get_element(Tensor *t, unsigned *index_list, int len_list);

double set_element(Tensor *t, unsigned *index_list, int len_list, double new_entry);

void extract_col(Tensor *out, Tensor *t, unsigned offset, int col);

double dot(Tensor *v1, Tensor *v2);

void single_operation(char op, Tensor *t1, double num);

void operation(Tensor *out, char op, Tensor *t1, Tensor *t2);

void matmul(Tensor *out, Tensor *t1, Tensor *t2);

double norm(Tensor *t);

// --------------------------------------------- algorithms

void QR(Tensor *Q, Tensor *R, Tensor *A);

// --------------------------------------------- print functions

void print_tensor(Tensor *t);

void print_shape(Tensor *t);

void print_stride(Tensor *t);

#endif // TENSOR_H_