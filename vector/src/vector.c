#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

int
vector_new(vector *v, size_t elem_size, vector_free_func free_func, int initial)
{
  if (elem_size == 0 || initial == 0) return VECT_INVALID_SIZE;
  v->elem_size = elem_size;
  v->step = initial;
  v->free_func = free_func;
  v->length = 0;
  v->alloc_length = initial;
  v->elems = calloc(initial, elem_size);
  return VECT_OK;
}

size_t
vector_length(const vector *v)
{
  return v->length;
}

static void
grow_if_needed(vector *v)
{
  if (v->length == v->alloc_length) {
    v->alloc_length *= v->step;
    v->elems = realloc(v->elems, v->alloc_length * v->elem_size);
  }
}

void
vector_append(vector *v, const void *elem_ptr)
{
  grow_if_needed(v);

  void *dst = (char *)v->elems + v->length * v->elem_size;
  memcpy(dst, elem_ptr, v->elem_size);
  v->length++;
}

int
vector_insert(vector *v, const void *elem_ptr, int position)
{
  if (position > (int)v->length || position < 0) {
    return VECT_INSERT_INVALID_POSITION;
  }

  if (v->length == 0 && position == 0) {
    vector_append(v, elem_ptr); /* avoid unnecessary memmove() */
    return VECT_OK;
  }

  grow_if_needed(v);

  void *src = (char *)v->elems + position * v->elem_size;
  void *dst = (char *)src + v->elem_size;
  memmove(dst, src, v->elem_size * (v->length - position));
  memcpy(src, elem_ptr, v->elem_size);
  v->length++;

  return VECT_OK;
}

static int
binary_search(const vector *v, const void *key, int start, int end, vector_cmp_func cmp_func)
{

  if (end < start) return VECT_SEARCH_NOT_FOUND;

  int cmp;
  int middle = (end > start) ? (end - start / 2) : end;
  void *elem = (char *)v->elems + middle * v->elem_size;

  cmp = cmp_func(key, elem);

  if (cmp < 0)
    return binary_search(v, key, start, middle-1, cmp_func);
  if (cmp > 0)
    return binary_search(v, key, middle+1, end, cmp_func);

  return middle;
}

int
vector_search(const vector *v, const void *key, vector_cmp_func cmp_func,
              int start, bool is_sorted)
{
  size_t i;

  if (key == NULL) {
    return VECT_SEARCH_INVALID_KEY;
  }

  if (start < 0 || start >= (int)v->length) {
    return VECT_SEARCH_INVALID_START;
  }

  if (is_sorted) {
    return binary_search(v, key, start, v->length-1, cmp_func);
  } else {
    for (i = start; i < v->length; i++) {
      if (cmp_func((char *)v->elems + i * v->elem_size, key) == 0) return i;
    }
  }
  return VECT_SEARCH_NOT_FOUND;
}


void *
vector_get(const vector *v, int position)
{
  if (position >= (int)v->length || position < 0)
    return NULL;
  return ((char *)v->elems + (position * v->elem_size));
}

int
vector_replace(vector *v, int position, const void *elem_ptr)
{
  if (position < 0 || position >= (int)v->length) {
    return VECT_REPLACE_INVALID_POSITION;
  }

  void *pos = (char *)v->elems + position * v->elem_size;

  if (v->free_func) {
    v->free_func(pos);
  }

  memcpy(pos, elem_ptr, v->elem_size);
  return VECT_OK;
}

void
vector_sort(vector *v, vector_cmp_func cmp_func)
{
  if (cmp_func == NULL) return;
  qsort(v->elems, v->length, v->elem_size, cmp_func);
}

int
vector_delete(vector *v, int position)
{
  if (position < 0 || position >= (int)v->length) {
    return VECT_DELETE_INVALID_POSITION;
  }

  void *elem = (char *)v->elems + position * v->elem_size;
  if (v->free_func != NULL) {
    v->free_func(elem);
  }

  if (position != ((int)v->length - 1) && (int)v->length > 1) {
    void *source = (char *)v->elems + (position+1) * v->elem_size;
    void *destin = (char *)v->elems + position * v->elem_size;
    size_t num = (v->length - (position+1)) * v->elem_size;
    memmove(destin, source, num);
  }

  v->length--;
  return VECT_OK;
}

void
vector_dispose(vector *v)
{
  if (v->free_func != NULL) {
    int i;
    for (i = 0; i < (int)v->length; i++) {
      v->free_func((char *)v->elems + i * v->elem_size);
    }
  }
  free(v->elems);
}
