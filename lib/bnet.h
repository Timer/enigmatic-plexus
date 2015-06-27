#include "list.h"

#ifndef BNET_H
#define BNET_H

typedef struct {
} CPD;

typedef struct {
  List *cpds;
} BayesianNetwork;

void bnet_delete(BayesianNetwork *b);

#endif
