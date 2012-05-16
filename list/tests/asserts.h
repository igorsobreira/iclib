#include <stdarg.h>

void assert_list_bounds(ic_list *list)
{
  fail_unless(list->head->prev == NULL, "list head's 'prev' should be NULL");
  fail_unless(list->tail->next == NULL, "list tail's 'next' should be NULL");
}

void assert_list_has_only_one_element(ic_list *list, void *element, size_t elem_size)
{
  void *head, *tail;

  fail_unless(ic_list_length(list) == 1);

  head = ic_list_first(list);
  tail = ic_list_last(list);

  fail_unless(head != NULL);
  fail_unless(tail != NULL);
  fail_unless(memcmp(element, head, elem_size) == 0, "element should be head");
  fail_unless(memcmp(element, tail, elem_size) == 0, "element should be tail");
}

void assert_list_elements(ic_list *list, ...)
{
  va_list argptr;
  void *element;
  size_t n = 0;

  va_start(argptr, list);

  while (n < ic_list_length(list)) {
    element = va_arg(argptr, void *);
    fail_unless(memcmp(element, ic_list_nth_data(list, n), sizeof(element)) == 0,
                "element at index %d doesn't match", n);
    n++;
  }

  va_end(argptr);
}
