//
// 2019-02-18. Author F. Bertrand <frederic.bertrand@utt.fr>
// Copyright (c) Universite de Strasbourg and CNRS
//

// for macos
// typedef long unsigned int size_t;

#ifndef SRC_UTILS_H
#define SRC_UTILS_H

void shuffle(int *array, size_t n);
int sample_int(int min, int max);
int sample(int size, double *probs);
void sampleFromGaussian(double mu, double sigma, double *Z);
void swap(int *array, int a, int b);
double generateRandomNumber(int min, int max);
int factorial(int n);
int sort(int *input, int *nodes, int size);
void permute(int *v, int start, int n, int **perm_array, int number_perm);

#endif //SRC_UTILS_H
