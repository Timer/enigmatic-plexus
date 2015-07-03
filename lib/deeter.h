#ifndef DEETER_H
#define DEETER_H

#include "matrix.h"

// return the index in the histogram of possible column variations
// given sz and a column
int count_index(Matrix* sz, Matrix* sample_data, int col);

#endif
