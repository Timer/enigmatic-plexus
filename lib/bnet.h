#include "list.h"
#include "matrix.h"

#ifndef BNET_H
#define BNET_H

typedef struct {
  Matrix *sizes;
  int dirichlet;
} CPD;

List * adjacency_matrix_parents(Matrix *adj_mat, int col);

#endif
