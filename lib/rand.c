#include <stdlib.h>
#include <assert.h>

unsigned int rand_interval(unsigned int min, unsigned int max) {
  assert(max >= min);
  int r;
  const unsigned int range = 1 + max - min;
  const unsigned int buckets = RAND_MAX / range;
  const unsigned int limit = buckets * range;

  do {
    r = rand();
  } while (r >= limit);

  return min + (r / buckets);
}

void shuffle_int(int c, int *a) {
  int b, d;
  while (c) b = rand_interval(0, --c), d = a[c], a[c] = a[b], a[b] = d;
}
