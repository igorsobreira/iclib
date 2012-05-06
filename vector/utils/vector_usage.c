#include <stdio.h>
#include "../src/vector.h"

int main(void)
{

  vector v;
  int num = 23;

  vector_new(&v, sizeof(int *), NULL, 2);

  vector_append(&v, &num);
  vector_append(&v, &num);
  vector_append(&v, &num);
  vector_append(&v, &num);

  vector_dispose(&v);

  return 0;
}
