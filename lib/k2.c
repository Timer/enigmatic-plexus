#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "matrix.h"
#include "bnet.h"

CPD * tabular_CPD(BayesianNetwork *bnet, int self, void *args) {
  return NULL;//TODO: this
}

BayesianNetwork * mk_bnet(Matrix *dag, void *ns, char *prefix, List *discrete) {
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

  BayesianNetwork *bnet = mk_bnet(dag, ns, "discrete", discrete);
  CPD *cpd;
  if (!strcmp(scoring_fn, "tabular")) {
    cpd = tabular_CPD(bnet, j, args);
  } else {
    assert(1 == 2);
  }
  //TODO: bnet->cpds[j] = cpd;
  return 0;//TODO: log_marg_prob_node(bnet.CPD{j}, data(j,:), data(ps,:))
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
        *matrix_element_by_index(pscore, pi) = score_family(j, ps, type, scoring_fn, ns, discrete, data, params);
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
