#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <check.h>
#include "../src/ic_list.h"
#include "asserts.h"

START_TEST (new_ic_list_should_be_empty)
{
  ic_list *mylist = ic_list_new();
  fail_unless(ic_list_empty(mylist), "new list should be empty");
  ic_list_free(mylist);
}
END_TEST

/* append */

START_TEST (append_one_element_should_make_it_head_and_tail)
{
  int num = 10;
  ic_list *mylist = ic_list_new();
  ic_list_append(mylist, &num);

  assert_list_has_only_one_element(mylist, &num, sizeof(int *));
  assert_list_bounds(mylist);

  ic_list_free(mylist);
}
END_TEST

START_TEST (append_many_elements)
{
  int num1 = 12, num2 = 30, num3 = 42;

  ic_list *mylist = ic_list_new();
  ic_list_append(mylist, &num1);
  ic_list_append(mylist, &num2);
  ic_list_append(mylist, &num3);

  fail_unless(ic_list_length(mylist), 3);
  assert_list_elements(mylist, &num1, &num2, &num3);
  assert_list_bounds(mylist);

  ic_list_free(mylist);
}
END_TEST

/* prepend */

START_TEST (prepend_one_element_should_make_it_head_and_tail)
{
  int num = 20;
  ic_list *mylist = ic_list_new();
  ic_list_prepend(mylist, &num);

  assert_list_has_only_one_element(mylist, &num, sizeof(int *));
  assert_list_bounds(mylist);

  ic_list_free(mylist);
}
END_TEST

START_TEST (prepend_many_elements)
{
  int num1 = 12, num2 = 30, num3 = 42;

  ic_list *mylist = ic_list_new();
  ic_list_prepend(mylist, &num3);
  ic_list_prepend(mylist, &num2);
  ic_list_prepend(mylist, &num1);

  fail_unless(ic_list_length(mylist), 3);
  assert_list_elements(mylist, &num1, &num2, &num3);
  assert_list_bounds(mylist);

  ic_list_free(mylist);
}
END_TEST

/* nth */

START_TEST (nth_should_return_element_at_that_position_or_NULL_if_out_of_bounds)
{
  int num1 = 1, num2 = 2;
  ic_list *mylist = ic_list_new();

  ic_list_append(mylist, &num1);
  ic_list_append(mylist, &num2);

  fail_unless(*(int *)ic_list_nth(mylist, 0) == num1);
  fail_unless(*(int *)ic_list_nth(mylist, 1) == num2);
  fail_unless(ic_list_nth(mylist, 2) == NULL);
  fail_unless(ic_list_nth(mylist, 10) == NULL);

  ic_list_free(mylist);
}
END_TEST

Suite *ic_list_suite(void) {
  Suite *s = suite_create("list");
  TCase *tc_list = tcase_create("list");

  tcase_add_test(tc_list, new_ic_list_should_be_empty);
  tcase_add_test(tc_list, append_one_element_should_make_it_head_and_tail);
  tcase_add_test(tc_list, append_many_elements);

  tcase_add_test(tc_list, prepend_one_element_should_make_it_head_and_tail);
  tcase_add_test(tc_list, prepend_many_elements);

  tcase_add_test(tc_list, nth_should_return_element_at_that_position_or_NULL_if_out_of_bounds);

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
