#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <float.h>

// --- PRIVATE START
int _matrix_index_for(Matrix *m, int row, int col) { return col * m->rows + row; }
// --- PRIVATE END

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

Matrix * matrix_create(int rows, int cols, int value) {
  Matrix *matrix = matrix_raw(rows, cols);
  void **data = matrix->data;
  for (int i = 0; i < rows * cols; ++i) {
    *((int *) (data[i] = malloc(sizeof(int)))) = value;
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

Matrix * matrix_double_create(int rows, int cols, double value) {
  Matrix *matrix = matrix_raw(rows, cols);
  void **data = matrix->data;
  for (int i = 0; i < rows * cols; ++i) {
    *((double *) (data[i] = malloc(sizeof(double)))) = value;
  }
  return matrix;
}

Matrix * matrix_range(int from, int to) {
  Matrix *matrix = matrix_zeros((to - from) + 1, 1);
  for (int i = from; i <= to; ++i) {
    *((int *) matrix_element_by_index(matrix, i - from)) = i;
  }
  return matrix;
}

void * matrix_element(Matrix *matrix, int row, int col) {
  if (row >= matrix->rows || col >= matrix->cols) {
    exit(EXIT_FAILURE);
  }
  return (matrix->data)[_matrix_index_for(matrix, row, col)];
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

int matrix_prod(Matrix *matrix) {
  int p = 1;
  void **data = matrix->data;
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) {
    p *= *(int *) data[i];
  }
  return p;
}

void matrix_dobule_set(Matrix *matrix, int value) {
  void **data = matrix->data;
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) {
    *((double *) (matrix->data)[i]) = value;
  }
}

Matrix * matrix_sub_indices(Matrix *o_matrix, int row_start, int row_end, int col_start, int col_end) {
  Matrix *n_matrix = matrix_raw(row_end - row_start, col_end - col_start);
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int c = col_start, ci = 0; c < col_end; ++c, ++ci) {
    assert(c < o_matrix->cols);
    for (int r = row_start, ri = 0; r < row_end; ++r, ++ri) {
      assert(r < o_matrix->rows);
      n_data[_matrix_index_for(n_matrix, ri, ci)] = o_data[_matrix_index_for(o_matrix, r, c)];
    }
  }
  return n_matrix;
}

Matrix * matrix_sub_lists(Matrix *o_matrix, List *rows, List *cols) {
  Matrix *n_matrix = matrix_raw(rows->count, cols->count);
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int ri = 0; ri < rows->count; ++ri) {
    for (int ci = 0, r = list_get_int(rows, ri); ci < cols->count; ++ci) {
      int c = list_get_int(cols, ci);
      assert(r < o_matrix->rows && c < o_matrix->cols);
      n_data[_matrix_index_for(n_matrix, ri, ci)] = o_data[_matrix_index_for(o_matrix, r, c)];
    }
  }
  return n_matrix;
}

Matrix * matrix_sub_list_index(Matrix *o_matrix, List *rows, int col_start, int col_end) {
  Matrix *n_matrix = matrix_raw(rows->count, col_end - col_start);
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int ri = 0; ri < rows->count; ++ri) {
    for (int c = col_start, ci = 0, r = list_get_int(rows, ri); c < col_end; ++c, ++ci) {
      assert(r < o_matrix->rows && c < o_matrix->cols);
      n_data[_matrix_index_for(n_matrix, ri, ci)] = o_data[_matrix_index_for(o_matrix, r, c)];
    }
  }
  return n_matrix;
}

Matrix * matrix_sub_index_list(Matrix *o_matrix, int row_start, int row_end, List *cols) {
  Matrix *n_matrix = matrix_raw(row_end - row_start, cols->count);
  void **n_data = n_matrix->data, **o_data = o_matrix->data;
  for (int r = row_start, ri = 0; r < row_end; ++r, ++ri) {
    for (int ci = 0; ci < cols->count; ++ci) {
      int c = list_get_int(cols, ci);
      assert(r < o_matrix->rows && c < o_matrix->cols);
      n_data[_matrix_index_for(n_matrix, ri, ci)] = o_data[_matrix_index_for(o_matrix, r, c)];
    }
  }
  return n_matrix;
}

Matrix * matrix_sub_col(Matrix *matrix, int col) {
  return matrix_sub_indices(matrix, 0, matrix->rows, col, col + 1);
}

Matrix * matrix_sub_row(Matrix *matrix, int row) {
  return matrix_sub_indices(matrix, row, row + 1, 0, matrix->cols);
}

Matrix * matrix_sub_concat_rows(Matrix *matrix, Matrix *rows) {
  assert(matrix->cols == rows->cols);
  Matrix *nm = matrix_raw(matrix->rows + rows->rows, matrix->cols);
  int **o_data = (int **) matrix->data, **n_data = (int **) nm->data,
    **a_data = (int **) rows->data;
  for (int i = 0, o_rows = matrix->rows; i < matrix->rows * matrix->cols; ++i) {
    *n_data++ = *o_data++;
    if (i != 0 && i % o_rows == 0) n_data += rows->rows;
  }
  n_data = (int **) nm->data + matrix->rows;
  for (int i = 0, o_rows = matrix->rows, a_rows = rows->rows; i < rows->rows * rows->cols; ++i) {
    *n_data++ = *a_data++;
    if (i != 0 && i % a_rows == 0) n_data += o_rows;
  }
  return nm;
}

List * matrix_find_by_value(Matrix *matrix, int value) {
  List *list = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    for (int r = 0; r < matrix->rows; ++r) {
      if (*((int *) matrix_element(matrix, r, c)) == value) list_push_int(list, _matrix_index_for(matrix, r, c));
    }
  }
  return list;
}

List * matrix_double_find_by_value(Matrix *matrix, double value) {
  List *list = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    for (int r = 0; r < matrix->rows; ++r) {
      if (*((double *) matrix_element(matrix, r, c)) == value) list_push_double(list, _matrix_index_for(matrix, r, c));
    }
  }
  return list;
}

MatrixMax * matrix_max(Matrix *matrix) {
  MatrixMax *max = malloc(sizeof(MatrixMax));
  max->cols = matrix->cols;
  max->values = list_empty();
  max->rows = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    int largest = INT_MIN, largest_row = -1;
    for (int r = 0; r < matrix->rows; ++r) {
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
  max->cols = matrix->cols;
  max->values = list_empty();
  max->rows = list_empty();
  for (int c = 0; c < matrix->cols; ++c) {
    double largest = -DBL_MAX;
    int largest_row = -1;
    for (int r = 0; r < matrix->rows; ++r) {
      double val = *((double *) matrix_element(matrix, r, c));
      if (val > largest) {
        largest = val;
        largest_row = r;
      }
    }
    list_push_double(max->values, largest);
    list_push_int(max->rows, largest_row);
  }
  return max;
}

List * matrix_to_list(Matrix *matrix) {
  List *l = list_empty();
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) list_push_int(l, *((int *) matrix_element_by_index(matrix, i)));
  return l;
}

List * matrix_double_to_list(Matrix *matrix) {
  List *l = list_empty();
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) list_push_double(l, *((double *) matrix_element_by_index(matrix, i)));
  return l;
}

Matrix * matrix_from_list(int rows, int cols, int arr[]) {
  Matrix *m = matrix_zeros(rows, cols);
  for (int i = 0; i < rows * cols; ++i) {
    *(int *) matrix_element_by_index(m, i) = arr[i];
  }
  return m;
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

// --- N-DIMENSIONAL START
void * matrix_element_n_dim(Matrix *m, Matrix *ind, Matrix *dims) {
  assert(m->rows * m->cols == matrix_prod(dims));
  assert(ind->rows * ind->cols == dims->rows * dims->cols);
  int index = 0;
  int **id = (int **) ind->data, **dd = (int **) dims->data;
  for (int i = 0, m = 1; i < ind->rows * ind->cols; m *= *dd[i++]) {
    assert(*id[i] < *dd[i]);
    index += *id[i] * m;
  }
  return matrix_element_by_index(m, index);
}
// --- N-DIMENSIONAL END

void matrix_display(Matrix *matrix) {
  for (int i = 0; i < matrix->rows * matrix->cols; ++i) {
    printf("[%d]", *((int *) matrix_element_by_index(matrix, i)));
  }
  printf("\n");
}

Matrix* matrix_add(Matrix* a, Matrix* b) {
  assert(a->rows == b->rows);
  assert(a->cols == b->cols);
  Matrix* r = matrix_zeros(a->rows, a->cols);
  int** ad = (int**) a->data, ** bd = (int**) b->data, ** rd = (int**) r->data;
  for (int i = 0; i < a->rows * a->cols; ++i)
    *rd[i] = *ad[i] + *bd[i];

  return r;
}

Matrix* matrix_add_double(Matrix* a, Matrix* b) {
  assert(a->rows == b->rows);
  assert(a->cols == b->cols);
  Matrix* r = matrix_zeros(a->rows, a->cols);
  double** ad = (double**) a->data, ** bd = (double**) b->data, ** rd = (double**) r->data;
  for (int i = 0; i < a->rows * a->cols; ++i)
    *rd[i] = *ad[i] + *bd[i];

  return r;
}

// first is int, second is double
Matrix* matrix_add_int_double(Matrix* a, Matrix* b) {
  assert(a->rows == b->rows);
  assert(a->cols == b->cols);
  Matrix* r = matrix_zeros(a->rows, a->cols);
  int** ad = (int**) a->data;
  double** bd = (double**) b->data, ** rd = (double**) r->data;
  for (int i = 0; i < a->rows * a->cols; ++i)
    *rd[i] = *ad[i] + *bd[i];

  return r;
}
