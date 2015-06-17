#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "matrix.h"

//TODO: node_type is cell array, ns still unknown
int score_family(int j, List *ps, void *node_type, char *scoring_fn, void *ns, List *discrete, Matrix *data) {
  int n = data->rows, ncases = data->cols;

  return 0;
}

Matrix * learn_struct_K2(
  Matrix *data, void *ns, List *order
) {
  assert(order->count == data->rows);
  int n = data->rows, ncases = data->cols;
  int max_fan_in = n;
  char *scoring_fn = "bayesian";
  List *discrete = list_empty();
  for (int i = 0; i < n; ++i) list_push_int(discrete, i);
  Matrix *clamped = matrix_zeros(n, ncases);

  Matrix *dag = matrix_zeros(n, n);
  for (int i = 0; i < n; ++i) {
    List *ps = list_empty();
    int j = list_get_int(order, i);
    Matrix *u_matrix = matrix_sub_indices(clamped, j, j + 1, 0, clamped->cols);
    assert(u_matrix->rows == 1);
    List *u = matrix_find_by_value(u_matrix, 0);
    matrix_scrap(u_matrix);
    Matrix *data_sub = matrix_sub_index_list(data, 0, n, u);
    //TODO: type{j}/params{j}
    int score = score_family(j, ps, NULL, scoring_fn, ns, discrete, data_sub);
    for (; ps->count <= max_fan_in ;) {
      List *order_sub = list_slice(order, 0, i - 1);
      List *pps = difference_type_int(order_sub, ps);
      list_scrap(order_sub);
      int nps = pps->count;
      Matrix *pscore = matrix_zeros(1, nps);
      for (int pi = 0; pi < nps; ++pi) {
        int p = list_get_int(pps, pi);
        int n_index = list_push_int(ps, p);
        *matrix_element_by_index(pscore, pi) =
        //TODO: type{j}/params{j}
          score_family(j, ps, NULL, scoring_fn, ns, discrete, data_sub);
        free(list_remove(ps, n_index));
      }
      matrix_scrap(data_sub);
      MatrixMax *mm = matrix_max(pscore);
      assert(mm->cols == 1);
      int best_pscore = list_get_int(mm->values, 0), best_p = list_get_int(mm->rows, 0);
      matrix_max_delete(mm);
      best_p = list_get_int(pps, best_p);
      list_scrap(pps);
      if (best_pscore > score) {
        score = best_pscore;
        list_push_int(ps, best_p);
      } else break;
    }
    Matrix *dag_sub = matrix_sub_list_index(dag, ps, j, j + 1);
    matrix_set(dag_sub, 1);
    matrix_scrap(dag_sub);
    list_delete(u);
    list_delete(ps);
  }
  matrix_delete(clamped);
  return dag;
}

int main(int argc, char **argv) {
  return 0;
}
