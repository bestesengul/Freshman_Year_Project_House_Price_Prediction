#include"dataset.h"
#ifndef _MODELS_H
#define _MODELS_H

// YONTEM 1
int model_by_similarity(House**houses,int size_houses, House* new_house);


// YONTEM 2
void make_prediction(House** test_list, int listSize, float* coefficients, int Cpitch, int Crow);

float* calculate_parameter(float* X, int Xpitch, int Xrow,
                         float* Y, int Ypitch, int Yrow);

int get_inverse(float *A, int Apitch, int Arow,
                float *I, int *Ipitch, int *Irow);

void adjoint(float* A, int Apitch, int Arow,
             float* S, int Spitch, int Srow);

void getCofactor(float* A, int Apitch, int Arow,
                 float* C, int *Cpitch, int *Crow);

int is_invertible(float* A, int Apitch, int Arow);

float getDeterminant(float* A, int Apitch, int Arow);

void get_multiplication_normal(float* A,int Apitch,  int Arow,
                               float* B,int Bpitch,  int Brow,
                               float* S,int* Spitch, int* Srow) ;

void get_transpose(float* A, int Apitch,  int Arow,
                   float* S, int* Spitch, int* Srow);


void print_data_matrice(float* matrice, int matricePitch, int row);

void create_data_matrice(House** houses, float* X, int Xpitch, int Xrow, 
                                         float* Y, int Ypitch, int Yrow);

#endif /* _MODELS_H */

