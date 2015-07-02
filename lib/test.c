#include <stdio.h>
#include "matrix.h"
#include "deeter.h"

int main() {
  Matrix* m = matrix_zeros(2, 3);
  int a = 0;
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j)
      *(matrix_element(m, i, j)) = a++;


  for (int i = 0; i < 6; ++i)
    printf("%d\n", *(matrix_element_by_index(m, i)));


  int b = prod(m);
  printf("%d", b);
  for (int i = 0; i < 3; ++i)
    printf("%d", *(matrix_element(m, 0, i)));
}
