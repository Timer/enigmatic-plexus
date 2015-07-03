#include <stdlib.h>
#include <stdio.h>
#include "deeter.h"

int prod(Matrix* sz) {
  // create return value
  // an empty matrix returns 1
  int ret = 1;

  // for each item in the matrix
  for (int i = 0; i < sz->rows * sz->cols; ++i)
    ret *= *(matrix_element_by_index(sz, i));
  return ret;
}

/*
  count_index will return the index of where to store the number of times
  a particular row was found in the prior data

  this is entirely dependent on the max number for each row (integers)

  so in our data if we have 3 samples, the max index for this array would be prod(sz)

  example for sz [2 2 2]
    1 1 1
    2 1 1
    1 2 1
    2 2 1
    1 1 2
    2 1 2
    1 2 2
    2 2 2

    If read from right to left this is in order if
    reduced by 1 and treated as bits

    this becomes

    1 1 1 --> 0 0 0 --> 0
    2 1 1 --> 0 0 1 --> 1
    etc

  example for sz [3 2 2]
    1 1 1
    2 1 1
    3 1 1
    1 2 1
    2 2 1
    3 2 1
    1 1 2
    2 1 2
    3 1 2
    1 2 2
    2 2 2
    3 2 2

  This is in base 3, how do we get 3?

    0 0 0
    0 0 1
    0 0 2
    0 1 0
    0 1 1
    0 1 2
    1 0 0
    1 0 1
    1 0 2
    1 1 0
    1 1 1
    1 1 2

  lets do one more
    sz [2 4 2]
    1 1 1 --> 0 0 0
    2 1 1 --> 0 0 1
    1 2 1 --> 0 1 0
    2 2 1 --> 0 1 1
    1 3 1 --> 0 2 0
    2 3 1 --> 0 2 1
    1 4 1 --> 0 3 0
    2 4 1 --> 0 3 1
    1 1 2 --> 1 0 0
    2 1 2 --> 1 0 1
    1 2 2 --> 1 1 0
    2 2 2 --> 1 1 1
    1 3 2 --> 1 2 0
    2 3 2 --> 1 2 1
    1 4 2 --> 1 3 0
    2 4 2 --> 1 3 1

    so how do we get the index?
    we know the total len is prod(sz)
    so let total' = prod(sz)
    then we /= by each in sz from right to left
    total' /= 2 --> 8, this is our first offset (when coming in from the right) so we take # - 1 * 8
    then we move the next chunk with WHAT IS REMAINING
    total' /= 4 --> 2 so we take the next # - 1 * 2
    then we use what is remainin again
    total' /= 2 --> 1, so we multiply our left most # - 1 * 1 and get our third #

    then just add them together

    so where does 1 3 2 go?
    2 -->  1 * 8 = 8
    3 -->  2 * 2 = 4
    1 -->  0 * 1 = 0

    8 + 4 + 0 = 12, which matches the previous example
*/
int count_index(Matrix* sz, Matrix* sample_data, int col) {
  // grab the desired column
  Matrix* mat_col = matrix_sub_col(sample_data, col);

  // the index
  int index = 0;

  // calculate the length of the histogram array
  int total = prod(sz);
  // move from the bottom of the column to the top
  for (int i = mat_col->rows - 1; i >= 0; --i) {
    // adjust the total for each sub-index
    total /= *matrix_element_by_index(sz, i);
    // add the new sub-index to index
    index += (*matrix_element_by_index(mat_col, i) - 1) * total;
  }
  return index;
}



Matrix* matrix_sub_col(Matrix *matrix, int col) {
  return matrix_sub_indices(matrix, 0, matrix->rows, col, col + 1);
}

Matrix* matrix_sub_row(Matrix *matrix, int row) {
  return matrix_sub_indices(matrix, row, row + 1, 0, matrix->cols);
}
