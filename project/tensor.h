#ifndef TENSOR_H_
#define TENSOR_H_

/*
 * This is a (kind of) efficient implementation of a n-d tensor analysis Libary:
 * It covers all base functionality required for the multi-dim QR decomposition,
 * which works on any tensor witch square matrices in the last to dimensions
 * (i.e. shape = [..., n, n]). It will perform QR decomposition of every Matrix
 * stored in the last tow dimensions of the Tensor. It will store the results
 * in two given Tensor *pointers, which will be converted in the right shape
 * befor the decomposition starts.
 *
 * A Tensor consists of the data, stride, shape, rank and the number of entries:
 *
 * The data is stored in a dynamic 1-d array. The multi-dimensionality is realized
 * by defining how given indices are converted into a flat index if the 1d-array.
 *
 * The stride defines how the respective index contributes to the calculation of
 * the 1-d index (i.e. stride:[12,1] => index:[2,3] = 2 * 12 + 1 * 3)
 *
 * The shape defines the size of each dimension of the Tensor.
 *
 * The rank defines how many dimensions the Tensor has.
 *
 * The number of entries is stored separate, since it is not possible to extract
 * the array size form a dynamic array.
 */

typedef struct
{
    unsigned num_entries;
    int rank;
    unsigned *shape;
    unsigned *stride;
    double *data;
} Tensor;

/*
 * helper function:
 * used to assist functions in the other modules.
 */

unsigned _fill_shape(Tensor *t, unsigned *shape, int rank);

unsigned _flatten_index(Tensor *t, unsigned *index_list, int len_list);

unsigned _reorder(Tensor *t, unsigned flat_index);

unsigned _reorder_three(Tensor *t, unsigned residual, unsigned x, unsigned y, unsigned z);

double _random();

/*
 * initialization functions:
 * used to initialize shape and/or fill tensors. Create a new Tensor *pointer exclusively with init_tensor()
 * and use the other functions to shape and fill the tensor.
 * subspace creates a tensor without allocating memory for the data. Instead it should use already allocated
 * memory from another tensor for its data.
 */

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

/*
 * shape functions:
 * used to change the dimensionality of tensors, whereby view, permute and transpose only change shape and stride,
 * therefore being very cheap to compute.
 * reshape and flat are O(num_entries) operations as they actually reorder the
 * data array.
 */

void reshape(Tensor *t, unsigned *shape, int rank);

void view(Tensor *t, unsigned *shape, int rank);

void permute(Tensor *t, int *permutation, int len_permutation);

void flat(Tensor *t);

void transpose(Tensor *t);

/*
 * base operations:
 * used for basic tensor computations
 */

double get_element(Tensor *t, unsigned *index_list, int len_list);

double set_element(Tensor *t, unsigned *index_list, int len_list, double new_entry);

void extract_col(Tensor *out, Tensor *t, unsigned offset, int col);

double dot(Tensor *v1, Tensor *v2);

void single_operation(char op, Tensor *t1, double num);

void operation(Tensor *out, char op, Tensor *t1, Tensor *t2);

void matmul(Tensor *out, Tensor *t1, Tensor *t2);

double norm(Tensor *t);

/*
 * algorithms:
 * used for more complex tensor algorithms like QR decomposition
 */

void QR(Tensor *Q, Tensor *R, Tensor *A);

/*
 * print functions:
 * print tensor, shape or stride in the comandline
 */

void print_tensor(Tensor *t);

void print_shape(Tensor *t);

void print_stride(Tensor *t);

#endif // TENSOR_H_