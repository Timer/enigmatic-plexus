#include "list.h"

#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  int rows, cols;
  void **data;
} Matrix;

typedef struct {
  int cols;
  List *values, *rows;
} MatrixMax;

Matrix * matrix_zeros(int rows, int cols);
Matrix * matrix_create(int rows, int cols, int value);
Matrix * matrix_double_zeros(int rows, int cols);
Matrix * matrix_double_create(int rows, int cols, double value);
Matrix * matrix_range(int from, int to);
void * matrix_element(Matrix *matrix, int row, int col);
void * matrix_element_by_index(Matrix *matrix, int index);
void matrix_set(Matrix *matrix, int value);
int matrix_prod(Matrix *matrix);
void matrix_double_set(Matrix *matrix, double value);
Matrix * matrix_sub_indices(Matrix *matrix, int row_start, int row_end, int col_start, int col_end);
Matrix * matrix_sub_lists(Matrix *matrix, List *rows, List *cols);
Matrix * matrix_sub_list_index(Matrix *matrix, List *rows, int col_start, int col_end);
Matrix * matrix_sub_index_list(Matrix *matrix, int row_start, int row_end, List *cols);
Matrix * matrix_sub_col(Matrix *matrix, int col);
Matrix * matrix_sub_row(Matrix *matrix, int row);
Matrix * matrix_sub_concat_rows(Matrix *matrix, Matrix *rows);
List * matrix_find_by_value(Matrix *matrix, int value);
List * matrix_double_find_by_value(Matrix *matrix, double value);
MatrixMax * matrix_max(Matrix *matrix);
MatrixMax * matrix_double_max(Matrix *matrix);
List * matrix_to_list(Matrix *matrix);
List * matrix_double_to_list(Matrix *matrix);
Matrix * matrix_from_list(int rows, int cols, int arr[]);
void matrix_max_delete(MatrixMax *matrix_max);
void matrix_delete(Matrix *matrix);
void matrix_scrap(Matrix *matrix);

void * matrix_element_n_dim(Matrix *m, Matrix *ind, Matrix *dims);

// for testing ease
void matrix_display();
#endif
