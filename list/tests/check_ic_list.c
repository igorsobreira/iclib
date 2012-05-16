#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <check.h>
#include "../src/ic_list.h"

START_TEST (new_ic_list_should_be_empty)
{
  ic_list *mylist = ic_list_new();
  fail_unless(ic_list_empty(mylist), "new list should be empty");
  ic_list_free(mylist);
}
END_TEST

START_TEST (append_one_element_should_make_it_head_and_tail)
{
  int num1 = 10;
  int *head, *tail;

  ic_list *mylist = ic_list_new();
  ic_list_append(mylist, &num1);

  head = ic_list_first(mylist);
  tail = ic_list_last(mylist);

  fail_unless(ic_list_length(mylist) == 1);

  fail_unless(head != NULL);
  fail_unless(*head == num1, "expected %d, found %d", num1, *head);

  fail_unless(tail != NULL);
  fail_unless(*tail == num1, "expected %d, found %d", num1, *tail);

  ic_list_free(mylist);
}
END_TEST

START_TEST (append_many_elements)
{
  int num1 = 12, num2 = 30, num3 = 42;
  int *head, *middle, *tail;

  ic_list *mylist = ic_list_new();
  ic_list_append(mylist, &num1);
  ic_list_append(mylist, &num2);
  ic_list_append(mylist, &num3);

  fail_unless(ic_list_length(mylist) == 3);

  head = ic_list_first(mylist);
  tail = ic_list_last(mylist);
  middle = mylist->head->next->data;

  fail_unless(head != NULL, "head should not me NULL");
  fail_unless(*head == num1, "expected %d, found %d", num1, *head);

  fail_unless(middle != NULL, "second element should not be NULL");
  fail_unless(*middle == num2, "expected %d, found %d", num2, *middle);

  fail_unless(tail != NULL, "tail should not be NULL");
  fail_unless(*tail == num3, "expected %d, found %d", num3, *tail);

  ic_list_free(mylist);
}
END_TEST

Suite *ic_list_suite(void) {
  Suite *s = suite_create("list");
  TCase *tc_list = tcase_create("list");

  tcase_add_test(tc_list, new_ic_list_should_be_empty);
  tcase_add_test(tc_list, append_one_element_should_make_it_head_and_tail);
  tcase_add_test(tc_list, append_many_elements);

  suite_add_tcase(s, tc_list);

  return s;
}

int main(void) {
  int nfailed;
  Suite *s = ic_list_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  nfailed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
