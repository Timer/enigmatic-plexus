#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "deeter.h"

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

  int index = 0;
  int **id = (int **) mat_col->data, **dd = (int **) sz->data;
  for (int i = 0, m = 1; i < mat_col->rows * mat_col->cols; m *= *dd[i++]) {
    assert((*id[i]) - 1 < *dd[i]);
    index += ((*id[i]) - 1) * m;
  }
  return index;
}

Matrix * compute_counts(Matrix* sample_data, Matrix* sz) {
  Matrix * count = matrix_zeros(matrix_prod(sz), 1);
   for (int i = 0; i < sample_data->cols; ++i) {
    *((int*) matrix_element_by_index(count, count_index(sz, sample_data, i))) += 1;
  }
  return count;
}
