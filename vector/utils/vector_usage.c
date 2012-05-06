#include <stdio.h>
#include "../src/vector.h"

int main(void)
{

  vector *v;
  int num = 23;

  v = vector_new(sizeof(int *), NULL, 2);

  vector_append(v, &num);
  vector_append(v, &num);
  vector_append(v, &num);
  vector_append(v, &num);

  vector_free(v);

  return 0;
}
