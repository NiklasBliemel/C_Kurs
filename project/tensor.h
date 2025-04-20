typedef struct
{
    unsigned int num_entries;
    int len_shape;
    unsigned int *shape;
    unsigned int *stride;
    double *data;
} Tensor;

Tensor *emtpy_tensor(unsigned int *shape, int len_shape);

Tensor *emtpy_like(Tensor *t);

void pop(Tensor *tensor);

Tensor *zeros(unsigned int *shape, int len_shape);

void print_tensor(Tensor *tensor);

void print_shape(Tensor *tensor);

void print_stride(Tensor *tensor);

double get_element(Tensor *tensor, unsigned int *index_list, int len_list);

double set_element(Tensor *tensor, unsigned int *index_list, int len_list, double new_entry);

void reshape(Tensor *tensor, unsigned int *shape, int len_shape);

void permute(Tensor *tensor, int *permutation, int len_permutation);

void flat(Tensor *t);

Tensor *copy(Tensor *tensor);

Tensor *arange(unsigned int range);

Tensor *_operation(char op, Tensor *t1, Tensor *t2);

Tensor *add(Tensor *t1, Tensor *t2);

Tensor *dot(Tensor *t1, Tensor *t2);
