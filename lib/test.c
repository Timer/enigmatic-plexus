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
      [1][3][1][2][2]
      [1][2][2][4][1]
      */
      Matrix* data = matrix_zeros(3, 5);
      *matrix_element_by_index(data, 0) = 2;
      *matrix_element_by_index(data, 1) = 1;
      *matrix_element_by_index(data, 2) = 1;
      *matrix_element_by_index(data, 3) = 1;
      *matrix_element_by_index(data, 4) = 3;
      *matrix_element_by_index(data, 5) = 2;
      *matrix_element_by_index(data, 6) = 2;
      *matrix_element_by_index(data, 7) = 1;
      *matrix_element_by_index(data, 8) = 2;
      *matrix_element_by_index(data, 9) = 1;
      *matrix_element_by_index(data, 10) = 2;
      *matrix_element_by_index(data, 11) = 4;
      *matrix_element_by_index(data, 12) = 1;
      *matrix_element_by_index(data, 13) = 2;
      *matrix_element_by_index(data, 14) = 1;

      printf("\n == created matrix == \n");
      for (int i = 0; i < data->rows; ++i) {
        for (int j = 0; j < data->cols; ++j)
          printf("[%d]", *matrix_element(data, i, j));
        printf("\n");
      }

      printf("\n == testing matrix_sub_indices, 1st col == \n");
      Matrix* second_col = matrix_sub_indices(data, 0, 3, 0, 1);
      printf("Rows: %d, Cols: %d\n", second_col->rows, second_col->cols);
      for (int i = 0; i < second_col->rows; ++i) {
        for (int j = 0; j < second_col->cols; ++j)
          printf("[%d]", *matrix_element(second_col, i, j));
        printf("\n");
      }
      matrix_scrap(second_col);

      printf("\n == testing matrix_sub_col, 4th col == \n");
      Matrix* fourth_col = matrix_sub_col(data, 4);
      printf("Rows: %d, Cols: %d\n", fourth_col->rows, fourth_col->cols);
      for (int i = 0; i < fourth_col->rows; ++i) {
        for (int j = 0; j < fourth_col->cols; ++j)
          printf("[%d]", *matrix_element(fourth_col, i, j));
        printf("\n");
      }
      matrix_scrap(fourth_col);

      printf("\n == testing matrix_sub_row, 2nd row == \n");
      Matrix* second_row = matrix_sub_row(data, 1);
      printf("Rows: %d, Cols: %d\n", second_row->rows, second_row->cols);
      for (int i = 0; i < second_row->rows; ++i) {
        for (int j = 0; j < second_row->cols; ++j)
          printf("[%d]", *matrix_element(second_row, i, j));
        printf("\n");
      }
      matrix_scrap(second_row);

      /* creating the following sz
      [2][3][4]
      */
      Matrix* sz = matrix_zeros(1, 3);
      *matrix_element_by_index(sz, 0) = 2;
      *matrix_element_by_index(sz, 1) = 3;
      *matrix_element_by_index(sz, 2) = 4;

      printf("\n == testing count_index == \n");
      count_index(sz, data, 1);


}
