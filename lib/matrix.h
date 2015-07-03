#include "list.h"

#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  int rows, cols, **data;
} Matrix;

typedef struct {
  int cols;
  List *values, *rows;
} MatrixMax;

Matrix * matrix_zeros(int rows, int cols);
int * matrix_element(Matrix *matrix, int row, int col);
int * matrix_element_by_index(Matrix *matrix, int index);
void matrix_set(Matrix *matrix, int value);
Matrix * matrix_sub_indices(Matrix *matrix, int row_start, int row_end, int col_start, int col_end);
Matrix * matrix_sub_lists(Matrix *matrix, List *rows, List *cols);
Matrix * matrix_sub_list_index(Matrix *matrix, List *rows, int col_start, int col_end);
Matrix * matrix_sub_index_list(Matrix *matrix, int row_start, int row_end, List *cols);
List * matrix_find_by_value(Matrix *matrix, int value);
MatrixMax * matrix_max(Matrix *matrix);
List * matrix_to_list(Matrix *matrix);
void matrix_max_delete(MatrixMax *matrix_max);
void matrix_delete(Matrix *matrix);
void matrix_scrap(Matrix *matrix);
void matrix_fill_with_list(List* values);

#endif
