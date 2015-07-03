#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "matrix.h"
#include "bnet.h"

int dirichlet_score_family(int counts, int prior) {
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

int compute_counts(Matrix *data, Matrix *sz) {
  assert(sz->cols == data->cols);
  /*
  P = prod(sz);
  indices = subv2ind(sz, data');
  count = hist(indices, 1:P);
  count = myreshape(count, sz);
  */
  return 0;//TODO: this
}

int log_marg_prob_node(CPD *cpd, Matrix *self_ev, Matrix *pev) {
  assert(self_ev->rows == 1);
  assert(pev->rows == 1);
  Matrix *data = matrix_zeros(2, self_ev->cols > pev->cols ? self_ev->cols : pev->cols);
  for (int i = 0; i < pev->cols; ++i) {
    *((int *) matrix_element(data, 0, i)) = *((int *) matrix_element(pev, 0, i));
  }
  for (int i = 0; i < self_ev->cols; ++i) {
    *((int *) matrix_element(data, 0, i)) = *((int *) matrix_element(self_ev, 0, i));
  }
  int counts = compute_counts(data, cpd->sizes);
  return dirichlet_score_family(counts, cpd->dirichlet);
}

CPD * tabular_CPD(Matrix *dag, void *node_sizes, int self, void *args) {
  void *ns = node_sizes;
  /*
  ps = parents(bnet.dag, self);
  fam_sz = ns([ps self]);
  CPD.sizes = fam_sz;

  psz = prod(ns(ps));
  dirichlet_weight = 1;
  CPD.dirichlet = (dirichlet_weight/psz) * mk_stochastic(myones(fam_sz));
  */
  return NULL;//TODO: this
}

int score_family(int j, List *ps, char *node_type, char *scoring_fn, void *ns, List *discrete, Matrix *data, void *args) {
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
    cpd = tabular_CPD(dag, ns, j, args);
  } else {
    assert(1 == 2);
  }
  Matrix *data_sub_1 = matrix_sub_indices(data, j, j + 1, 0, data->cols),
    *data_sub_2 = matrix_sub_list_index(data, ps, 0, data->cols);
  int score = log_marg_prob_node(cpd, data_sub_1, data_sub_2);
  matrix_scrap(data_sub_1);
  matrix_scrap(data_sub_2);
  return score;
}

Matrix * learn_struct_K2(
  Matrix *data, void *ns, List *order
) {
  assert(order->count == data->rows);
  int n = data->rows, ncases = data->cols;
  int max_fan_in = n;
  char *type = "tabular";
  char *scoring_fn = "bayesian";
  char *params = "";//TODO: set this different
  List *discrete = list_empty();
  for (int i = 0; i < n; ++i) list_push_int(discrete, i);

  Matrix *dag = matrix_zeros(n, n);
  for (int i = 0; i < n; ++i) {
    List *ps = list_empty();
    int j = list_get_int(order, i);
    int score = score_family(j, ps, type, scoring_fn, ns, discrete, data, params);
    for (; ps->count <= max_fan_in ;) {
      List *order_sub = list_slice(order, 0, i - 1);
      List *pps = difference_type_int(order_sub, ps);
      list_scrap(order_sub);
      int nps = pps->count;
      Matrix *pscore = matrix_zeros(1, nps);
      for (int pi = 0; pi < nps; ++pi) {
        int p = list_get_int(pps, pi);
        int n_index = list_push_int(ps, p);
        *((int *) matrix_element_by_index(pscore, pi)) = score_family(j, ps, type, scoring_fn, ns, discrete, data, params);
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
