#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "matrix.h"

int score_family() {
  return 0;
}

Matrix * learn_struct_K2(
  Matrix *data, List *order
) {
  assert(order->count == data->rows);
  int n = data->rows, ncases = data->cols;
  int max_fan_in = n;
  Matrix *clamped = matrix_zeros(n, ncases);

  Matrix *dag = matrix_zeros(n, n);
  for (int i = 0; i < n; ++i) {
    List *ps = list_empty();
    int j = list_get_int(order, i);
    Matrix * u_matrix = matrix_sub_indices(clamped, j, j + 1, 0, clamped->cols);
    assert(u_matrix->rows == 1);
    List *u = matrix_find_by_value(u_matrix, 0);
    matrix_scrap(u_matrix);
    int score = score_family();
    //TODO: score = score_family(j, ps, type{j}, scoring_fn, ns, discrete, data(:,u), params{j});
    for (; ps->count <= max_fan_in ;) {
      List *pps = list_empty();
      //TODO: pps = mysetdiff(order(1:i-1), ps);
      int nps = pps->count;
      Matrix *pscore = matrix_zeros(1, nps);
      for (int pi = 0; pi < nps; ++pi) {
        int p = list_get_int(pps, pi);
        *matrix_element_by_index(pscore, pi) = score_family();
        //TODO: pscore(pi) = score_family(j, [ps p], type{j}, scoring_fn, ns, discrete, data(:,u), params{j});
      }
      MatrixMax *mm = matrix_max(pscore);
      assert(mm->cols == 1);
      int best_pscore = list_get_int(mm->values, 0), best_p = list_get_int(mm->rows, 0);
      matrix_max_delete(mm);
      best_p = list_get_int(pps, best_p);
      list_delete(pps);
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
