#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "list.h"
#include "deeter.h"

int main(int argc, char* argv[]) {

      Matrix* m = matrix_zeros(2, 3);
      int a = 1;
      for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
          *(matrix_element(m, i, j)) = a++;

      printf("\n == matrix by index == \n");
      for (int i = 0; i < 6; ++i)
        printf("[%d]", *(matrix_element_by_index(m, i)));
      printf("\n");

      printf("\n == matrix by row/col, top row == \n");
      for (int i = 0; i < 3; ++i)
        printf("[%d]", *(matrix_element(m, 0, i)));
      printf("\n");

      printf("\n == prod == \n");
      int b = prod(m);
      printf("%d", b);
      printf("\n");

      matrix_delete(m);

      /* creating the following matrix
      [2][1][2][1][1]
      [1][1][1][2][2]
      [1][2][2][1][1]
      */
      Matrix* data = matrix_zeros(3, 5);
      *matrix_element_by_index(data, 0) = 2;
      *matrix_element_by_index(data, 1) = 1;
      *matrix_element_by_index(data, 2) = 1;
      *matrix_element_by_index(data, 3) = 1;
      *matrix_element_by_index(data, 4) = 1;
      *matrix_element_by_index(data, 5) = 2;
      *matrix_element_by_index(data, 6) = 2;
      *matrix_element_by_index(data, 7) = 1;
      *matrix_element_by_index(data, 8) = 2;
      *matrix_element_by_index(data, 9) = 1;
      *matrix_element_by_index(data, 10) = 2;
      *matrix_element_by_index(data, 11) = 1;
      *matrix_element_by_index(data, 12) = 1;
      *matrix_element_by_index(data, 13) = 2;
      *matrix_element_by_index(data, 14) = 1;

      printf("\n == created matrix == \n");
      for (int i = 0; i < data->rows; ++i) {
        for (int j = 0; j < data->cols; ++j)
          printf("[%d]", *matrix_element(data, i, j));
        printf("\n");
      }
      printf("\n");

      printf("\n == testing matrix_sub_indices == \n");
      Matrix* second_col = matrix_sub_indices(data, 0, 3, 0, 1);
      printf("Rows: %d, Cols: %d\n", second_col->rows, second_col->cols);
      for (int i = 0; i < second_col->rows; ++i) {
        for (int j = 0; j < second_col->cols; ++j)
          printf("[%d]", *matrix_element(second_col, i, j));
        printf("\n");
      }

}
