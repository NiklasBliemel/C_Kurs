#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tensor.h"

/* Projektarbeit C-Kurs, Blockkurs Frühling 2025
 * Kursteilnehmer:
 *    Niklas Bliemel, Matrikelnummer 2273004
 *
 * Projektname:
 *    N-dim Tensor QR decomposition
 *
 * Compileraufruf:
 *    make
 *
 * Programm:
 *    ./QR (Demo)
 *    ./QR -file.bin (reads tensor stored in file.bin an writes QR decomposition in file_Q.bin and file_R.bin, use test.bin for testing)
 *
 * Benotung:
 *    ich würde doch gerne um eine Benotung bitten falls das noch möglich ist, da ich mir am ende doch einen größeren
 *    Aufwand als geplant gemacht habe und zusätzlich zur QR decomposition eine ganze n-dim Tensor libary geschrieben habe :)
 *    Falls es nicht möglich, ist es auch oke!!
 */

void demo()
{
    // Demo for multi-dimensional QR decomposition:

    Tensor *A = init_tensor();
    Tensor *Q = init_tensor();
    Tensor *R = init_tensor();
    Tensor *A_test = init_tensor();
    Tensor *Q_t = init_tensor();
    Tensor *Q_test = init_tensor();

    unsigned shape[3] = {2, 3, 3};
    rands(A, shape, 3);

    QR(Q, R, A);

    // Test if QR = A
    matmul(A_test, Q, R);

    // Test if Q Q_t = 1
    copy(Q_t, Q);
    transpose(Q_t);
    matmul(Q_test, Q, Q_t);

    printf("A:\n");
    print_tensor(A);
    printf("\nQ:\n");
    print_tensor(Q);
    printf("\nR:\n");
    print_tensor(R);
    printf("\nQR:\n");
    print_tensor(A_test);
    printf("\nQQ_t:\n");
    print_tensor(Q_test);
    printf("\n");

    pop(A);
    pop(Q);
    pop(R);
    pop(A_test);
    pop(Q_t);
    pop(Q_test);
}

int ends_with_bin(const char *filename)
{
    size_t len = strlen(filename);
    if (len < 4)
    {
        return 0;
    }
    return strcmp(filename + len - 4, ".bin") == 0;
}

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        demo();
        return 0;
    }
    if (argc > 2)
    {
        printf("Only one file!\n");
        return 1;
    }
    int len_string = strlen(argv[1]);
    char *filename = malloc(len_string * sizeof(char));
    strcpy(filename, argv[1]);

    if (!ends_with_bin(filename))
    {
        printf("File has to end with \".bin\" !\n");
        return 1;
    }
    Tensor *A = init_tensor();
    Tensor *Q = init_tensor();
    Tensor *R = init_tensor();

    load_tensor(A, filename);
    QR(Q, R, A);

    filename = realloc(filename, (len_string + 2) * sizeof(char));
    printf("%s\n", filename);
    for (int i = len_string + 1; i >= len_string - 2; i--)
    {
        filename[i] = filename[i - 2];
    }
    filename[len_string - 4] = '_';

    filename[len_string - 3] = 'Q';
    save_tensor(Q, filename);
    filename[len_string - 3] = 'R';
    save_tensor(R, filename);

    printf("A:\n");
    print_tensor(A);
    printf("\nQ:\n");
    print_tensor(Q);
    printf("\nR:\n");
    print_tensor(R);

    pop(A);
    pop(Q);
    pop(R);
}
