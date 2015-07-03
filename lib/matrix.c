#include "matrix.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

Matrix * matrix_raw(int rows, int cols) {
  assert(rows != 0);
  assert(cols != 0);
  Matrix *matrix = malloc(sizeof(Matrix));
  matrix->rows = rows;
  matrix->cols = cols;
  matrix->data = malloc(rows * cols * sizeof(void *));
  return matrix;
}

Matrix * matrix_zeros(int rows, int cols) {
  Matrix *matrix = matrix_raw(rows, cols);
  void **data = matrix->data;
  for (int i = 0; i < rows * cols; ++i) {
    *((int *) (data[i] = malloc(sizeof(int)))) = 0;
  }
  return matrix;
}

Matrix * matrix_double_zeros(int rows, int cols) {
  Matrix *matrix = matrix_raw(rows, cols);
  void **data = matrix->data;
  for (int i = 0; i < rows * cols; ++i) {
    *((double *) (data[i] = malloc(sizeof(double)))) = 0;
  }
  return matrix;
}

void * matrix_element(Matrix *matrix, int row, int col) {
  if (row >= matrix->rows || col >= matrix->cols) {
    exit(EXIT_FAILURE);
  }
  return (matrix->data)[col * matrix->rows + row];
}

void * matrix_element_by_index(Matrix *matrix, int index) {
  if (index >= matrix->rows * matrix->cols) {
    exit(EXIT_FAILURE);
  }
  return (matrix->data)[index];
}

void matrix_set(Matrix *matrix, int value) {
  void **data = matrix->data;
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) {
    *((int *) (matrix->data)[i]) = value;
  }
}

void matrix_dobule_set(Matrix *matrix, int value) {
  void **data = matrix->data;
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) {
    *((double *) (matrix->data)[i]) = value;
  }
}

Matrix * matrix_sub_indices(Matrix *o_matrix, int row_start, int row_end, int col_start, int col_end) {
  Matrix *n_matrix = matrix_raw(row_end - row_start, col_end - col_start);
  int index = -1;
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int c = col_start; c < col_end; ++c) {
    int b = c * o_matrix->rows;
    for (int r = row_start; r < row_end; ++r) {
      n_data[++index] = o_data[b + r];
    }
  }
  return n_matrix;
}

// WE NEED TO FIX THIS

Matrix * matrix_sub_lists(Matrix *o_matrix, List *rows, List *cols) {
  Matrix *n_matrix = matrix_raw(rows->count, cols->count);
  int index = -1;
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int ri = 0; ri < rows->count; ++ri) {
    int r = list_get_int(rows, ri), b = r * o_matrix->cols;
    for (int ci = 0; ci < cols->count; ++ci) {
      int c = list_get_int(cols, ci);
      n_data[++index] = o_data[b + c];
    }
  }
  return n_matrix;
}

Matrix * matrix_sub_list_index(Matrix *o_matrix, List *rows, int col_start, int col_end) {
  Matrix *n_matrix = matrix_raw(rows->count, col_end - col_start);
  int index = -1;
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int ri = 0; ri < rows->count; ++ri) {
    int r = list_get_int(rows, ri), b = r * o_matrix->cols;
    for (int c = col_start; c < col_end; ++c) {
      n_data[++index] = o_data[b + c];
    }
  }
  return n_matrix;
}

Matrix * matrix_sub_index_list(Matrix *o_matrix, int row_start, int row_end, List *cols) {
  Matrix *n_matrix = matrix_raw(row_end - row_start, cols->count);
  int index = -1;
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int r = row_start; r < row_end; ++r) {
    int b = r * o_matrix->cols;
    for (int ci = 0; ci < cols->count; ++ci) {
      int c = list_get_int(cols, ci);
      n_data[++index] = o_data[b + c];
    }
  }
  return n_matrix;
}

List * matrix_find_by_value(Matrix *matrix, int value) {
  int index = 0;
  List *list = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    for (int r = 0; c < matrix->rows; ++r) {
      if (*((int *) matrix_element(matrix, r, c)) == value) list_push_int(list, index);
      ++index;
    }
  }
  return list;
}

List * matrix_double_find_by_value(Matrix *matrix, double value) {
  int index = 0;
  List *list = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    for (int r = 0; c < matrix->rows; ++r) {
      if (*((double *) matrix_element(matrix, r, c)) == value) list_push_double(list, index);
      ++index;
    }
  }
  return list;
}

MatrixMax * matrix_max(Matrix *matrix) {
  MatrixMax *max = malloc(sizeof(MatrixMax));
  max->values = list_empty();
  max->rows = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    int largest = INT_MIN, largest_row = -1;
    for (int r = 0; c < matrix->rows; ++r) {
      int val = *((int *) matrix_element(matrix, r, c));
      if (val > largest) {
        largest = val;
        largest_row = r;
      }
    }
    list_push_int(max->values, largest);
    list_push_int(max->rows, largest_row);
  }
  return max;
}

MatrixMax * matrix_double_max(Matrix *matrix) {
  MatrixMax *max = malloc(sizeof(MatrixMax));
  max->values = list_empty();
  max->rows = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    double largest = INT_MIN, largest_row = -1;
    for (int r = 0; c < matrix->rows; ++r) {
      double val = *((double *) matrix_element(matrix, r, c));
      if (val > largest) {
        largest = val;
        largest_row = r;
      }
    }
    list_push_double(max->values, largest);
    list_push_double(max->rows, largest_row);
  }
  return max;
}

List * matrix_to_list(Matrix *matrix) {
  assert(matrix->rows == 1);
  List *l = list_empty();
  for (int i = 0; i < matrix->cols; ++i) list_push_int(l, *((int *) matrix_element(matrix, 0, i)));
  return l;
}

List * matrix_double_to_list(Matrix *matrix) {
  assert(matrix->rows == 1);
  List *l = list_empty();
  for (int i = 0; i < matrix->cols; ++i) list_push_double(l, *((double *) matrix_element(matrix, 0, i)));
  return l;
}

void matrix_max_delete(MatrixMax *matrix_max) {
  list_delete(matrix_max->values);
  list_delete(matrix_max->rows);
  free(matrix_max);
}

void matrix_delete(Matrix *matrix) {
  for (int index = 0; index < matrix->rows * matrix->cols; ++index) {
    free((matrix->data)[index]);
  }
  matrix_scrap(matrix);
}

void matrix_scrap(Matrix *matrix) {
  free(matrix->data);
  matrix->data = NULL;
  free(matrix);
}
