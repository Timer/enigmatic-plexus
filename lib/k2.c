#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "matrix.h"
#include "bnet.h"

int dirichlet_score_family(Matrix *counts, Matrix* prior) {
  /*
  ns = mysize(counts);
  ns_ps = ns(1:end-1);
  ns_self = ns(end);
  if nargin < 2, prior = normalise(myones(ns)); end
  prior = reshape(prior(:), [prod(ns_ps) ns_self]);
  counts = reshape(counts,  [prod(ns_ps) ns_self]);
  LU = sum(gammaln(prior + counts) - gammaln(prior), 2);
  alpha_ij = sum(prior, 2);
  N_ij = sum(counts, 2);
  LV = gammaln(alpha_ij) - gammaln(alpha_ij + N_ij);
  LL = sum(LU + LV);
  */
  return 0;//TODO: this
}

int count_index(Matrix *sz, Matrix *sample_data, int col) {
  Matrix *mat_col = matrix_sub_col(sample_data, col);
  int index = 0;
  int **id = (int **) mat_col->data, **dd = (int **) sz->data;
  for (int i = 0, m = 1; i < mat_col->rows * mat_col->cols; m *= *dd[i++]) {
    assert((*id[i]) - 1 < *dd[i]);
    index += ((*id[i]) - 1) * m;
  }
  return index;
}

Matrix * compute_counts(Matrix *data, Matrix *sz) {
  assert(sz->cols == data->cols);
  Matrix *count = matrix_zeros(matrix_prod(sz), 1);
   for (int i = 0; i < data->cols; ++i) {
    *((int*) matrix_element_by_index(count, count_index(sz, data, i))) += 1;
  }
  return count;
}

int log_marg_prob_node(CPD *cpd, Matrix *self_ev, Matrix *pev) {
  assert(self_ev->rows == 1);
  assert(cpd->sizes->rows == 1);
  assert(pev->cols == self_ev->cols);
  Matrix *data = matrix_sub_concat_rows(pev, self_ev);
  Matrix *counts = compute_counts(data, cpd->sizes);
  matrix_scrap(data);
  const int score = dirichlet_score_family(counts, cpd->dirichlet);
  matrix_delete(counts);
  return score;
}

CPD * tabular_CPD(Matrix *dag, Matrix *ns, int self) {
  CPD *cpd = malloc(sizeof(CPD));
  List *ps = adjacency_matrix_parents(dag, self);
  list_push_int(ps, self);
  Matrix *fam_sz = matrix_zeros(1, ps->count);
  for (int i = 0; i < ps->count; ++i) {
    *(int *) matrix_element_by_index(fam_sz, i) = *(int *) matrix_element_by_index(ns, list_get_int(ps, i));
  }
  cpd->sizes = fam_sz;

  Matrix *calc = matrix_sub_indices(fam_sz, 0, 1, 0, ps->count - 1);
  int psz = matrix_prod(calc), dirichlet_weight = 1;
  list_delete(ps);
  matrix_scrap(calc);
  cpd->dirichlet = matrix_double_create(matrix_prod(fam_sz), 1, (1 / psz) * (1 / (*(int*) matrix_element_by_index(ns, self))));
  return cpd;
}

int score_family(int j, List *ps, char *node_type, char *scoring_fn, Matrix *ns, List *discrete, Matrix *data) {
  int n = data->rows, ncases = data->cols;
  Matrix *dag = matrix_zeros(0, 0);
  if (ps->count > 0) {
    Matrix *dag_sub = matrix_sub_list_index(dag, ps, j, j + 1);
    matrix_set(dag_sub, 1);
    matrix_scrap(dag_sub);
    //TODO: sort `ps` here.
  }

  CPD *cpd;
  if (!strcmp(scoring_fn, "tabular")) {
    cpd = tabular_CPD(dag, ns, j);
  } else {
    assert(1 == 2);
  }
  Matrix *data_sub_1 = matrix_sub_indices(data, j, j + 1, 0, data->cols),
    *data_sub_2 = matrix_sub_list_index(data, ps, 0, data->cols);
  int score = log_marg_prob_node(cpd, data_sub_1, data_sub_2);
  cpd_delete(cpd);
  matrix_scrap(data_sub_1);
  matrix_scrap(data_sub_2);
  return score;
}

Matrix * learn_struct_K2(
  Matrix *data, Matrix *ns, List *order
) {
  assert(order->count == data->rows);
  int n = data->rows, ncases = data->cols;
  int max_fan_in = n;
  char *type = "tabular";
  char *scoring_fn = "bayesian";
  List *discrete = list_empty();
  for (int i = 0; i < n; ++i) list_push_int(discrete, i);

  Matrix *dag = matrix_zeros(n, n);
  for (int i = 0; i < n; ++i) {
    List *ps = list_empty();
    int j = list_get_int(order, i);
    int score = score_family(j, ps, type, scoring_fn, ns, discrete, data);
    for (; ps->count <= max_fan_in ;) {
      List *order_sub = list_slice(order, 0, i - 1);
      List *pps = difference_type_int(order_sub, ps);
      list_scrap(order_sub);
      int nps = pps->count;
      Matrix *pscore = matrix_zeros(1, nps);
      for (int pi = 0; pi < nps; ++pi) {
        int p = list_get_int(pps, pi);
        int n_index = list_push_int(ps, p);
        *((int *) matrix_element_by_index(pscore, pi)) = score_family(j, ps, type, scoring_fn, ns, discrete, data);
        free(list_remove(ps, n_index));
      }
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
    if (ps->count > 0) {
      Matrix *dag_sub = matrix_sub_list_index(dag, ps, j, j + 1);
      matrix_set(dag_sub, 1);
      matrix_scrap(dag_sub);
    }
    list_delete(ps);
  }
  return dag;
}

int main(int argc, char **argv) {
  return 0;
}
