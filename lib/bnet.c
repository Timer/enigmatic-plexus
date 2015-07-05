#include "bnet.h"
#include <assert.h>
#include "matrix.h"

List * adjacency_matrix_parents(Matrix *adj_mat, int col) {
  List *l = list_empty();
  Matrix *sub = matrix_sub_indices(adj_mat, 0, adj_mat->rows, col, col + 1);
  for (int i = 0; i < adj_mat->rows; ++i) {
    const int val = *(int *) matrix_element_by_index(adj_mat, i);
    assert(val == 0 || val == 1);
    if (val) list_push_int(l, i);
  }
  matrix_scrap(sub);
  return l;
}
