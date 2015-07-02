#include <stdio.h>
#include "matrix.h"
#include "deeter.h"

int main() {
  Matrix* m = matrix_zeros(2, 3);
  for (int i = 0; i < 6; ++i)
    *(matrix_element_by_index(m, i)) = i + 1;


  for (int i = 0; i < 6; ++i)
    printf("%d\n", *(matrix_element_by_index(m, i)));


  int b = prod(m);
  printf("%d", b);
}
