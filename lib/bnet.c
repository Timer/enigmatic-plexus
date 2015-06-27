#include "bnet.h"
#include <stdlib.h>

void bnet_delete(BayesianNetwork *b) {
  list_delete(b->cpds);
  free(b);
}
