#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrix.h"
#include "list.h"
#include "deeter.h"

void test_helpers() {
  // --- MATRIX START
  puts("Creating test matrix ...");
  Matrix *m = matrix_zeros(3, 4);

  puts("Validating matrix set ...");
  matrix_set(m, 10);
  for (int i = 0; i < m->rows * m->cols; ++i) {
    assert(*(int *) matrix_element_by_index(m, i) == 10);
  }

  puts("Validating element and index correspondence ...");
  const int test_row = 1, test_col = 2, test_index = 7;
  *(int *) matrix_element(m, test_row, test_col) *= 2;
  assert(*(int *) matrix_element_by_index(m, test_index) == 20);

  puts("Testing matrix find ...");
  List *l = matrix_find_by_value(m, 20);
  assert(l->count == 1);
  assert(list_get_int(l, 0) == test_index);
  list_delete(l);

  puts("Validating proper generation of sub matrices ...");
  Matrix *sm = matrix_sub_indices(m, 1, 2, 1, 3);
  assert(sm->rows == 1 && sm->cols == 2);
  matrix_set(sm, 5);
  matrix_scrap(sm);

  l = matrix_find_by_value(m, 5);
  assert(l->count == 2);
  assert(list_get_int(l, 0) == 4);
  assert(list_get_int(l, 1) == 7);
  list_delete(l);

  List *rows = list_empty(), *cols = list_empty();
  list_push_int(rows, 1);
  list_push_int(rows, 2);
  list_push_int(cols, 3);
  sm = matrix_sub_lists(m, rows, cols);
  assert(sm->rows == 2 && sm->cols == 1);
  matrix_set(sm, 6);
  matrix_scrap(sm);

  l = matrix_find_by_value(m, 6);
  assert(l->count == 2);
  assert(list_get_int(l, 0) == 10);
  assert(list_get_int(l, 1) == 11);
  list_delete(l);

  sm = matrix_sub_index_list(m, 1, 3, cols);
  assert(sm->rows == 2 && sm->cols == 1);
  matrix_set(sm, 7);
  matrix_scrap(sm);

  l = matrix_find_by_value(m, 7);
  assert(l->count == 2);
  assert(list_get_int(l, 0) == 10);
  assert(list_get_int(l, 1) == 11);
  list_delete(l);

  sm = matrix_sub_list_index(m, rows, 3, 4);
  assert(sm->rows == 2 && sm->cols == 1);
  matrix_set(sm, 18);
  matrix_scrap(sm);

  l = matrix_find_by_value(m, 18);
  assert(l->count == 2);
  assert(list_get_int(l, 0) == 10);
  assert(list_get_int(l, 1) == 11);
  list_delete(l);

  puts("Testing matrix max ...");
  MatrixMax *mm = matrix_max(m);
  assert(mm->cols == 4);
  assert(mm->values->count == 4 && mm->rows->count == 4);
  assert(
    list_get_int(mm->values, 0) == 10 &&
    list_get_int(mm->values, 1) == 10 &&
    list_get_int(mm->values, 2) == 10 &&
    list_get_int(mm->values, 3) == 18
  );
  assert(
    list_get_int(mm->rows, 0) == 0 &&
    list_get_int(mm->rows, 1) == 0 &&
    list_get_int(mm->rows, 2) == 0 &&
    list_get_int(mm->rows, 3) == 1
  );
  matrix_max_delete(mm);
  // --- MATRIX END

  matrix_delete(m);
  puts("All tests passed!");
}

int main(int argc, char* argv[]) {
  test_helpers();

      Matrix* m = matrix_zeros(2, 3);
      int a = 1;
      for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
          *((int *) matrix_element(m, i, j)) = a++;

      printf("\n == matrix by index == \n");
      for (int i = 0; i < 6; ++i)
        printf("[%d]", *((int *) matrix_element_by_index(m, i)));
      printf("\n");

      printf("\n == matrix by row/col, top row == \n");
      for (int i = 0; i < 3; ++i)
        printf("[%d]", *((int *) matrix_element(m, 0, i)));
      printf("\n");

      printf("\n == prod == \n");
      int b = prod(m);
      printf("%d", b);
      printf("\n");

      matrix_delete(m);

      /* creating the following matrix
      [2][1][2][1][1][2]
      [1][3][1][2][2][3]
      [1][2][2][4][1][4]
      */
      Matrix* data = matrix_zeros(3, 6);
      *(int *) matrix_element_by_index(data, 0) = 2;
      *(int *) matrix_element_by_index(data, 1) = 1;
      *(int *) matrix_element_by_index(data, 2) = 1;
      *(int *) matrix_element_by_index(data, 3) = 1;
      *(int *) matrix_element_by_index(data, 4) = 3;
      *(int *) matrix_element_by_index(data, 5) = 2;
      *(int *) matrix_element_by_index(data, 6) = 2;
      *(int *) matrix_element_by_index(data, 7) = 1;
      *(int *) matrix_element_by_index(data, 8) = 2;
      *(int *) matrix_element_by_index(data, 9) = 1;
      *(int *) matrix_element_by_index(data, 10) = 2;
      *(int *) matrix_element_by_index(data, 11) = 4;
      *(int *) matrix_element_by_index(data, 12) = 1;
      *(int *) matrix_element_by_index(data, 13) = 2;
      *(int *) matrix_element_by_index(data, 14) = 1;
      *(int *) matrix_element_by_index(data, 15) = 2;
      *(int *) matrix_element_by_index(data, 16) = 3;
      *(int *) matrix_element_by_index(data, 17) = 4;

      printf("\n == created matrix == \n");
      for (int i = 0; i < data->rows; ++i) {
        for (int j = 0; j < data->cols; ++j)
          printf("[%d]", *(int *) matrix_element(data, i, j));
        printf("\n");
      }

      printf("\n == testing matrix_sub_indices, 1st col == \n");
      Matrix* second_col = matrix_sub_indices(data, 0, 3, 0, 1);
      printf("Rows: %d, Cols: %d\n", second_col->rows, second_col->cols);
      for (int i = 0; i < second_col->rows; ++i) {
        for (int j = 0; j < second_col->cols; ++j)
          printf("[%d]", *(int *) matrix_element(second_col, i, j));
        printf("\n");
      }
      matrix_scrap(second_col);

      printf("\n == testing matrix_sub_col, 4th col == \n");
      Matrix* fourth_col = matrix_sub_col(data, 4);
      printf("Rows: %d, Cols: %d\n", fourth_col->rows, fourth_col->cols);
      for (int i = 0; i < fourth_col->rows; ++i) {
        for (int j = 0; j < fourth_col->cols; ++j)
          printf("[%d]", *(int *) matrix_element(fourth_col, i, j));
        printf("\n");
      }
      matrix_scrap(fourth_col);

      printf("\n == testing matrix_sub_row, 2nd row == \n");
      Matrix* second_row = matrix_sub_row(data, 1);
      printf("Rows: %d, Cols: %d\n", second_row->rows, second_row->cols);
      for (int i = 0; i < second_row->rows; ++i) {
        for (int j = 0; j < second_row->cols; ++j)
          printf("[%d]", *(int *) matrix_element(second_row, i, j));
        printf("\n");
      }
      matrix_scrap(second_row);

      /* creating the following sz
      [2][3][4]
      */
      Matrix* sz = matrix_zeros(1, 3);
      *(int *) matrix_element_by_index(sz, 0) = 2;
      *(int *) matrix_element_by_index(sz, 1) = 3;
      *(int *) matrix_element_by_index(sz, 2) = 4;

      printf("\n == testing count_index == \n");
      printf("prod(sz): %d\n", prod(sz));
      for (int i = 0; i < data->cols; ++i)
        printf("index for col %d: %d\n", i, count_index(sz, data, i));
}
