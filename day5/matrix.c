#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int rows;
    int cols;
    double *data;
} *matrix;

matrix NewMat(int rows, int cols)
{
    double *data = malloc(rows * cols * sizeof(double));
    matrix mat = malloc(sizeof(*mat));
    mat->data = data;
    mat->cols = cols;
    mat->rows = rows;
    return mat;
}

void MatDelete(matrix mat)
{
    free(mat->data);
    free(mat);
}

void mat_set(matrix mat, int row, int col, double element)
{
    mat->data[row + mat->rows * col] = element;
}

double mat_get(matrix mat, int row, int col)
{
    return mat->data[row + mat->rows * col];
}

void print_mat(matrix mat)
{
    for (int i = 0; i < mat->rows; i++)
    {
        for (int j = 0; j < mat->cols - 1; j++)
        {
            printf("%lf,\t", mat_get(mat, i, j));
        }
        printf("%lf\n\n", mat_get(mat, i, mat->cols - 1));
    }
}

int main(void)
{
    int rows = 5;
    int cols = 5;
    matrix mat = NewMat(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat_set(mat, i, j, cols * i + j);
        }
    }
    print_mat(mat);
}
