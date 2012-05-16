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

  if (ic_list_empty(l)) {
    n->prev = NULL;
    n->next = NULL;
    l->head = n;
    l->tail = n;
  } else {
    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
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
