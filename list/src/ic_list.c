#include <stdlib.h>
#include "ic_list.h"

ic_list * ic_list_new(void)
{
  ic_list *l = malloc(sizeof(ic_list));
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
  return l;
}

bool ic_list_empty(ic_list *l)
{
  return l->head == NULL;
}

size_t ic_list_length(ic_list *l)
{
  return l->length;
}

void ic_list_append(ic_list *l, void *data)
{
  ic_node *n = malloc(sizeof(ic_node));
  n->data = data;
  n->next = NULL;

  if (ic_list_empty(l)) {
    n->prev = NULL;
    l->head = n;
    l->tail = n;
  } else {
    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
  }
  l->length++;
}

void ic_list_prepend(ic_list *l, void *data)
{
  ic_node *n = malloc(sizeof(ic_node));
  n->data = data;
  n->prev = NULL;

  if (ic_list_empty(l)) {
    l->head = n;
    l->tail = n;
    n->next = NULL;
  } else {
    n->next = l->head;
    l->head = n;
  }
  l->length++;
}

void * ic_list_first(ic_list *l)
{
  if (l->head != NULL)
    return l->head->data;
  return NULL;
}

void * ic_list_last(ic_list *l)
{
  if (l->tail != NULL)
    return l->tail->data;
  return NULL;
}

ic_node * ic_list_nth(ic_list *l, size_t n)
{
  ic_node *tmp, *node = l->head;
  size_t jumps = 0;

  if (n >= ic_list_length(l)) return NULL;

  while (jumps < n) {
    tmp = node->next;
    node = tmp;
    jumps++;
  }

  return node;
}

void * ic_list_nth_data(ic_list *l, size_t n)
{
  ic_node *node = ic_list_nth(l, n);
  if (node == NULL) return NULL;
  return node->data;
}

void ic_list_free(ic_list *l)
{
  if (ic_list_empty(l)) {
    free(l);
  } else {
    ic_node *tmp;
    while (l->head != l->tail) {
      tmp = l->head;
      l->head = l->head->next;
      free(tmp);
    }
    free(l->head);
    free(l);
  }
}
