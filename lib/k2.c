#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include <omp.h>
#include "list.h"
#include "matrix.h"
#include "bnet.h"

double dirichlet_score_family(Matrix *counts, CPD *cpd) {
  Matrix *ns = cpd->sizes, *prior = cpd->dirichlet;
  Matrix *ns_self = matrix_sub_indices(ns, ns->rows - 1, ns->rows, 0, ns->cols);
  Matrix *pnc = matrix_add_int_double(counts, prior);
  Matrix *gamma_pnc = matrix_lgamma(pnc), *gamma_prior = matrix_lgamma(prior);
  matrix_delete(pnc);
  Matrix *lu_mat = matrix_double_subtract(gamma_pnc, gamma_prior);
  matrix_delete(gamma_pnc);
  matrix_delete(gamma_prior);
  Matrix *LU = matrix_sum_n_cols_double(lu_mat, *(int *) matrix_element_by_index(ns_self, 0));
  matrix_delete(lu_mat);
  Matrix *alpha_ij = matrix_sum_n_cols_double(prior, *(int *) matrix_element_by_index(ns_self, 0));
  Matrix *N_ij = matrix_sum_n_cols(counts, *(int *) matrix_element_by_index(ns_self, 0));
  matrix_scrap(ns_self);
  Matrix *gamma_alpha = matrix_lgamma(alpha_ij);
  Matrix *alpha_N = matrix_add_int_double(N_ij, alpha_ij);
  matrix_delete(N_ij);
  matrix_delete(alpha_ij);
  Matrix *gamma_alpha_N = matrix_lgamma(alpha_N);
  matrix_delete(alpha_N);
  Matrix *LV = matrix_double_subtract(gamma_alpha, gamma_alpha_N);
  matrix_delete(gamma_alpha);
  matrix_delete(gamma_alpha_N);
  Matrix *LU_LV = matrix_add_double(LU, LV);
  matrix_delete(LU);
  matrix_delete(LV);
  double score = matrix_sum_double(LU_LV);
  matrix_delete(LU_LV);
  return score;
}

int count_index(Matrix *sz, Matrix *sample_data, int col) {
  Matrix *mat_col = matrix_sub_col(sample_data, col);
  int index = 0;
  int **id = (int **) mat_col->data, **dd = (int **) sz->data;
  for (int i = 0, m = 1; i < mat_col->rows * mat_col->cols; m *= *dd[i++]) {
    assert((*id[i]) - 1 < *dd[i]);
    index += ((*id[i]) - 1) * m;
  }
  matrix_scrap(mat_col);
  return index;
}

Matrix *compute_counts(Matrix *data, Matrix *sz) {
  assert(sz->rows == data->rows);
  Matrix *count = matrix_zeros(matrix_prod(sz), 1);
  for (int i = 0; i < data->cols; ++i) {
    *((int *) matrix_element_by_index(count, count_index(sz, data, i))) += 1;
  }
  return count;
}

double log_marg_prob_node(CPD *cpd, Matrix *self_ev, Matrix *pev) {
  assert(self_ev->rows == 1);
  assert(cpd->sizes->cols == 1);
  assert(pev->cols == self_ev->cols);
  Matrix *data = matrix_sub_concat_rows(pev, self_ev);
  Matrix *counts = compute_counts(data, cpd->sizes);
  matrix_scrap(data);
  const double score = dirichlet_score_family(counts, cpd);
  matrix_delete(counts);
  return score;
}

CPD *tabular_CPD(Matrix *dag, Matrix *ns, int self) {
  CPD *cpd = malloc(sizeof(CPD));
  List *ps = adjacency_matrix_parents(dag, self);
  list_push_int(ps, self);
  Matrix *fam_sz = matrix_zeros(ps->count, 1);
  for (int i = 0; i < ps->count; ++i) {
    *(int *) matrix_element_by_index(fam_sz, i) = *(int *) matrix_element_by_index(ns, list_get_int(ps, i));
  }
  cpd->sizes = fam_sz;
  Matrix *calc = matrix_sub_indices(fam_sz, 0, ps->count - 1, 0, 1);
  int psz = matrix_prod(calc);
  list_delete(ps);
  matrix_scrap(calc);
  cpd->dirichlet = matrix_double_create(matrix_prod(fam_sz), 1, (1.0 / psz) * (1.0 / *(int *) matrix_element_by_index(ns, self)));
  return cpd;
}

double score_family(int j, List *ps, Matrix *ns, List *discrete, Matrix *data) {
  Matrix *dag = matrix_zeros(data->rows, data->rows);
  if (ps->count > 0) {
    Matrix *dag_sub = matrix_sub_list_index(dag, ps, j, j + 1);
    matrix_set(dag_sub, 1);
    matrix_scrap(dag_sub);
    //TODO: sort `ps` here.
  }
  CPD *cpd = tabular_CPD(dag, ns, j);
  Matrix *data_sub_1 = matrix_sub_indices(data, j, j + 1, 0, data->cols),
         *data_sub_2 = matrix_sub_list_index(data, ps, 0, data->cols);
  double score = log_marg_prob_node(cpd, data_sub_1, data_sub_2);
  cpd_delete(cpd);
  matrix_scrap(data_sub_1);
  matrix_scrap(data_sub_2);
  matrix_delete(dag);
  return score;
}

Matrix *learn_struct_K2(Matrix *data, Matrix *ns, List *order) {
  assert(order->count == data->rows);
  int n = data->rows;
  int max_fan_in = n;
  List *discrete = list_empty();
  for (int i = 0; i < n; ++i) list_push_int(discrete, i);

  Matrix *dag = matrix_zeros(n, n);
  int parent_order = 0;
  for (int i = 0; i < n; ++i) {
    List *ps = list_empty();
    int j = list_get_int(order, i);
    double score = score_family(j, ps, ns, discrete, data);
    for (; ps->count <= max_fan_in;) {
      List *order_sub = list_slice(order, 0, i);
      List *pps = difference_type_int(order_sub, ps);
      list_scrap(order_sub);
      int nps = pps->count;
      Matrix *pscore = matrix_double_zeros(1, nps);
      for (int pi = 0; pi < nps; ++pi) {
        int p = list_get_int(pps, pi);
        int n_index = list_push_int(ps, p);
        *((double *) matrix_element_by_index(pscore, pi)) = score_family(j, ps, ns, discrete, data);
        free(list_remove(ps, n_index));
      }
      double best_pscore = -DBL_MAX;
      int best_p = -1;
      for (int i = 0; i < nps; ++i) {
        double d = *(double *) matrix_element_by_index(pscore, i);
        if (d > best_pscore) {
          best_pscore = d;
          best_p = i;
        }
      }
      matrix_delete(pscore);
      if (best_p == -1) {
        list_scrap(pps);
        break;
      }
      best_p = list_get_int(pps, best_p);
      list_scrap(pps);
      if (best_pscore > score) {
        score = best_pscore;
        list_push_int(ps, best_p);
      } else {
        break;
      }
    }
    if (ps->count > 0) {
      Matrix *dag_sub = matrix_sub_list_index(dag, ps, j, j + 1);
      matrix_set(dag_sub, ++parent_order);
      matrix_scrap(dag_sub);
    }
    list_delete(ps);
  }
  list_delete(discrete);
  return dag;
}

int exec(char *f_data, char *f_topologies, char *f_output) {
  Matrix *data = matrix_from_file(f_data), *sz = matrix_create_sz(data);
  Matrix *orders = matrix_from_file(f_topologies);

  int out_csv_row_count = data->rows;
  int out_csv_row_count_sq = out_csv_row_count * out_csv_row_count;

  FILE *csv = fopen(f_output, "w");
  fclose(csv);

  for (int o = 0; o < orders->rows; ++o) {
    Matrix *m_order = matrix_sub_indices(orders, o, o + 1, 0, orders->cols);
    List *order = matrix_to_list(m_order);
    Matrix *bnet = learn_struct_K2(data, sz, order);
    csv = fopen(f_output, "a");
    fprintf(csv, "%d:", bnet->rows);
    for (int i = 0; i < out_csv_row_count - 1; ++i) {
      fprintf(csv, "%d,", *(int *) matrix_element_by_index(m_order, i));
    }
    fprintf(csv, "%d:", *(int *) matrix_element_by_index(m_order, out_csv_row_count - 1));
    for (int i = 0; i < out_csv_row_count_sq - 1; ++i) {
      fprintf(csv, "%d,", *(int *) matrix_element_by_index(bnet, i));
    }
    fprintf(csv, "%d\n", *(int *) matrix_element_by_index(bnet, out_csv_row_count_sq - 1));
    fclose(csv);

    matrix_delete(bnet);
    list_delete(order);
    matrix_scrap(m_order);
  }

  printf("Wrote networks to %s\n", f_output);
  matrix_delete(orders);
  matrix_delete(sz);
  matrix_delete(data);
  return 0;
}

int main(int argc, char **argv) {
  int threads = 1;
  char *data = NULL, *topologies = NULL, *output = "consensus.csv";
  int c;
  while ((c = getopt(argc, argv, "p:d:t:o:")) != -1) {
    switch (c) {
    case 'p': {
      threads = atoi(optarg);
      assert(threads > 0);
      assert(threads <= omp_get_num_procs());
      break;
    }
    case 'd': {
      data = optarg;
      break;
    }
    case 't': {
      topologies = optarg;
      break;
    }
    case 'o': {
      output = optarg;
      break;
    }
    default: {
      return 1;
    }
    }
  }
  if (data == NULL) {
    puts("You must send a data file using -d <file name>.");
    return 1;
  }
  if (topologies == NULL) {
    puts("You must send topologies using -t <file name>.");
    return 1;
  }
  return exec(data, topologies, output);
}
