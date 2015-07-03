#ifndef DEETER_H
#define DEETER_H

#include "matrix.h"

// return the index in the histogram of possible column variations
// given sz and a column
int count_index(Matrix* sz, Matrix* sample_data, int col);


// matrix.h

// sub matrix of an entire column, DO NOT DELETE, USE SCRAP
Matrix* matrix_sub_col(Matrix *matrix, int col);

// sub matrix of an entire row, DO NOT DELETE, USE SCRAP
Matrix* matrix_sub_row(Matrix *matrix, int row);


#endif
