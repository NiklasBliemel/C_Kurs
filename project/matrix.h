typedef struct
{
    unsigned int *shape;
    unsigned int *index_factors;
    unsigned int num_entries;
    double *data;
} Tensor;

unsigned int *zero_list(int len);

Tensor *emtpy_tensor(unsigned int *shape);

void pop(Tensor *tensor);

Tensor *zeros(unsigned int *shape);

Tensor *zeros_like(Tensor *tensor);

Tensor *ones(unsigned int *shape);

Tensor *ones_like(Tensor *tensor);

Tensor *tensor(unsigned int *shape, double *data);

void print_tensor(Tensor *tensor);

void print_shape(Tensor *tensor);

double get_element(Tensor *tensor, unsigned int *index_list);

double set_element(Tensor *tensor, unsigned int *index_list, double new_entry);
