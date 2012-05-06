# iclib

Library with useful C data structures

## vector

A vector is type agnostic structure similar to a list that grows as needed.

### Running tests

Install [check](http://check.sourceforge.net/), unit tests framework.
Now you can run the lib tests:

    $ cd vector
    $ make

### Usage example

This example creates a vector of strings with initial 10 positions allocated:

```c
#include <stdio.h>
#include <string.h>
#include "vector.h"

void free_string(void *string) {
  free(*(char **)string);
}

int main(void) {
  vector v;
  vector_new(&v, sizeof(char *), free_string, 10);
  
  char *name = strdup("Igor");
  vector_append(&v, &name);
  
  printf("%s \n", *(char **)vector_get(&v, 0));

  vector_dispose(&v);
  return EXIT_SUCCESS;
}
```