#ifndef NETWORKABC_ALLOC2D_H
#define NETWORKABC_ALLOC2D_H

#include <stddef.h>

int  alloc2d_contig(double ***data, double **block_out, int rows, int cols);
/* Libère le bloc contigu PUIS le tableau de pointeurs de lignes */
void free2d_contig(double **data, double *block);

#endif