#include <stdlib.h>
#include "alloc2d.h"

int alloc2d_contig(double ***data, double **block_out, int rows, int cols) {
    if (rows <= 0 || cols <= 0) return 1;

    /* vérif overflow (par prudence) */
    size_t n = (size_t)rows * (size_t)cols;
    if (cols != 0 && n / (size_t)cols != (size_t)rows) return 2;

    double **rowptrs = (double**) malloc((size_t)rows * sizeof(double*));
    if (!rowptrs) return 3;

    double *block = (double*) calloc(n, sizeof(double));
    if (!block) { free(rowptrs); return 4; }

    for (int i = 0; i < rows; ++i)
        rowptrs[i] = block + (size_t)i * (size_t)cols;

    *data = rowptrs;
    *block_out = block;
    return 0;
}

void free2d_contig(double **data, double *block) {
    free(block);
    free(data);
}