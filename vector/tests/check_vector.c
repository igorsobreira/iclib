#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <check.h>
#include "../src/vector.h"

void free_string(void *str)
{
  free(*(char **)str);
}

int compare_ints(const void *num1, const void *num2)
{
  if (*(int *)num1 > *(int *)num2) return  1;
  if (*(int *)num1 < *(int *)num2) return -1;
  return 0;
}

void upper_string(void *string, void *data)
{
  char *str = *(char **)string;
  int i;
  for (i = 0; i < (int)strlen(str); i++)
    str[i] = toupper(str[i]);

  if (data != NULL) {
    char *aux = (char *)data;
    *aux = toupper(*aux);
  }
}

START_TEST (new_should_fill_struct_with_defaults)
{
  vector *v = vector_new(sizeof(int), NULL, 10);

  fail_unless(v->elem_size == sizeof(int),
              ".elem_size field should be sizeof(int)");
  fail_unless(v->free_func == NULL,
              ".free_func field should be NULL");
  fail_unless(v->step == 10,
              ".step field should be 10");
  fail_unless(v->length == 0,
              ".length should be zero");
  fail_unless(v->alloc_length == 10,
              ".alloc_length should be 10");
  fail_if(v->elems == NULL,
          ".elems should be `malloc`ed for the correct size");

  vector_free(v);
}
END_TEST

START_TEST (new_should_return_error_if_invalid_elem_size_or_initial)
{
  vector *v;

  v = vector_new(0, NULL, 10);
  fail_unless(v == NULL, "should fail if invalid size");

  v = vector_new(sizeof(int), NULL, 0);
  fail_unless(v == NULL, "should fail if invalid initial");

  vector_free(v);
}
END_TEST


START_TEST (should_append_and_get_one_element)
{
  int num = 42;
  int *found = NULL;

  vector *v = vector_new(sizeof(int *), NULL, 5);
  vector_append(v, &num);

  fail_unless(vector_length(v) == 1, "length should be 1");

  found = vector_get(v, 0);

  fail_if(found == NULL, "get() should not return NULL (or nothing)");
  fail_unless(*found == 42, "get() should return first element");

  vector_free(v);
}
END_TEST

START_TEST (should_append_and_get_multiple_elements)
{

  int num1 = 21, num2 = 37;
  int *found1, *found2;

  vector *v = vector_new(sizeof(int *), NULL, 3);
  vector_append(v, &num1);
  vector_append(v, &num2);
  found1 = vector_get(v, 0);
  found2 = vector_get(v, 1);

  fail_unless(vector_length(v) == 2, "length should be 2");
  fail_if(found1 == NULL, "found element 0 should not be NULL");
  fail_if(found2 == NULL, "found element 1 should not be NULL");
  fail_unless(*found1 == 21, "invalid element found at 0: %d", *found1);
  fail_unless(*found2 == 37, "invalid element found at 1: %d", *found2);

  vector_free(v);
}
END_TEST

START_TEST (append_should_grown_if_needed)
{

  int num1 = 10, num2 = 20, num3 = 30, num4 = 40;
  int *found1, *found2, *found3, *found4;

  vector *v = vector_new(sizeof(int *), NULL, 2);
  vector_append(v, &num1);
  vector_append(v, &num2);
  vector_append(v, &num3);   /* grow before append */
  vector_append(v, &num4);

  found1 = vector_get(v, 0);
  found2 = vector_get(v, 1);
  found3 = vector_get(v, 2);
  found4 = vector_get(v, 3);

  fail_unless(vector_length(v) == 4, "length should be 4");
  fail_unless(v->alloc_length == 4, ".alloc_length should be 10");
  fail_unless(*found1 == 10, "element 0 should be 10 not %d", *found1);
  fail_unless(*found2 == 20, "element 1 should be 20 not %d", *found2);
  fail_unless(*found3 == 30, "element 2 should be 30 not %d", *found3);
  fail_unless(*found4 == 40, "element 3 should be 40 not %d", *found4);

  vector_free(v);
}
END_TEST

START_TEST (get_should_fail_if_invalid_index)
{

  int *found1, *found2;

  vector *v = vector_new(sizeof(int *), NULL, 2);

  found1 = vector_get(v, 1);
  fail_unless(found1 == NULL, "elem not found should return NULL");

  found2 = vector_get(v, -3);
  fail_unless(found2 == NULL, "negative index should return NULL");

  vector_free(v);
}
END_TEST

START_TEST (should_use_free_function_to_dealloc_elemns)
{

  char *py = strdup("Python"), *rb = strdup("Ruby");
  char **found;

  vector *v = vector_new(sizeof(char *), free_string, 5);
  vector_append(v, &py);
  vector_append(v, &rb);
  found = vector_get(v, 1);

  fail_unless(*found == rb);

  vector_free(v);

}
END_TEST

START_TEST (insert_element_in_the_beginning)
{

  char *py = strdup("python");
  char *rb = strdup("ruby");
  char *lp = strdup("lisp");
  int rc;

  vector *v = vector_new(sizeof(char *), free_string, 5);
  vector_append(v, &py);
  vector_append(v, &rb);
  rc = vector_insert(v, &lp, 0);

  fail_unless(rc == 0);
  fail_unless(vector_length(v) == 3);
  fail_unless(lp == *(char **)vector_get(v, 0));
  fail_unless(py == *(char **)vector_get(v, 1));
  fail_unless(rb == *(char **)vector_get(v, 2));

  vector_free(v);
}
END_TEST

START_TEST (insert_elements_in_all_positions)
{

  char *move1 = strdup("forward loop");
  char *move2 = strdup("goiter");
  char *move3 = strdup("flaka");
  char *move4 = strdup("back loop");

  vector *v = vector_new(sizeof(char *), free_string, 5);
  fail_if(vector_insert(v, &move1, 0) == -1);
  fail_if(vector_insert(v, &move4, 1) == -1);
  fail_if(vector_insert(v, &move2, 1) == -1);
  fail_if(vector_insert(v, &move3, 2) == -1);

  fail_unless(4 == vector_length(v));
  fail_unless(move1 == *(char **)vector_get(v, 0));
  fail_unless(move2 == *(char **)vector_get(v, 1));
  fail_unless(move3 == *(char **)vector_get(v, 2));
  fail_unless(move4 == *(char **)vector_get(v, 3));

  vector_free(v);
}
END_TEST

START_TEST (insert_fails_if_position_bigger_than_length_or_negative)
{

  int num = 10;
  int rc;

  vector *v = vector_new(sizeof(int *), NULL, 5);

  rc = vector_insert(v, &num, 4); /* bigger than logical length */
  fail_unless(rc == VECT_INSERT_INVALID_POSITION);
  fail_unless(vector_length(v) == 0);

  rc = vector_insert(v, &num, -2);
  fail_unless(rc == VECT_INSERT_INVALID_POSITION);
  fail_unless(vector_length(v) == 0);

  vector_free(v);
}
END_TEST

START_TEST (insert_should_grow_if_needed)
{

  int num1 = 1, num2 = 2, num3 = 3, num4 = 4;

  vector *v = vector_new(sizeof(int *), NULL, 2);

  fail_unless(vector_insert(v, &num4, 0) == 0);
  fail_unless(vector_insert(v, &num3, 0) == 0);
  fail_unless(vector_insert(v, &num2, 0) == 0); /* grow before insert */
  fail_unless(vector_insert(v, &num1, 0) == 0);

  fail_unless(4 == vector_length(v));
  fail_unless(1 == *(int *)vector_get(v, 0));
  fail_unless(2 == *(int *)vector_get(v, 1));
  fail_unless(3 == *(int *)vector_get(v, 2));
  fail_unless(4 == *(int *)vector_get(v, 3));

  vector_free(v);
}
END_TEST

START_TEST (search_should_return_element_position)
{

  int num1 = 12, num2 = 22, num3 = 23;

  vector *v = vector_new(sizeof(int *), NULL, 5);
  vector_append(v, &num1);
  vector_append(v, &num2);
  vector_append(v, &num3);

  fail_unless(vector_search(v, &num1, compare_ints, 0, false) == 0);
  fail_unless(vector_search(v, &num2, compare_ints, 0, false) == 1);
  fail_unless(vector_search(v, &num3, compare_ints, 0, false) == 2);

  fail_unless(vector_search(v, &num1, compare_ints, 0, true) == 0);
  fail_unless(vector_search(v, &num2, compare_ints, 0, true) == 1);
  fail_unless(vector_search(v, &num3, compare_ints, 0, true) == 2);

  vector_free(v);
}
END_TEST

START_TEST (search_should_fail_if_element_not_found)
{

  int num1 = 12, num2 = 22, num3 = 42;

  vector *v = vector_new(sizeof(int *), NULL, 5);
  vector_append(v, &num1);
  vector_append(v, &num2);

  fail_unless(vector_search(v, &num3, compare_ints, 0, false) == VECT_SEARCH_NOT_FOUND);
  fail_unless(vector_search(v, &num3, compare_ints, 0, true) == VECT_SEARCH_NOT_FOUND);

  vector_free(v);
}
END_TEST

START_TEST (search_should_fail_if_invalid_key)
{


  vector *v = vector_new(sizeof(int *), NULL, 5);
  fail_unless(vector_search(v, NULL, compare_ints, 0, false) == VECT_SEARCH_INVALID_KEY);

  vector_free(v);
}
END_TEST

START_TEST (search_should_ignore_everything_before_start_parameter)
{

  int num1 = 12, num2 = 22, num3 = 23, num4 = 30, num5 = 34;

  vector *v = vector_new(sizeof(int *), NULL, 5);
  vector_append(v, &num1);
  vector_append(v, &num2);
  vector_append(v, &num3);
  vector_append(v, &num4);
  vector_append(v, &num5);

  fail_unless(vector_search(v, &num1, compare_ints, 1, false) == VECT_SEARCH_NOT_FOUND);
  fail_unless(vector_search(v, &num2, compare_ints, 2, false) == VECT_SEARCH_NOT_FOUND);
  fail_unless(vector_search(v, &num5, compare_ints, 2, false) == 4);

  fail_unless(vector_search(v, &num1, compare_ints, 1, true) == VECT_SEARCH_NOT_FOUND);
  fail_unless(vector_search(v, &num2, compare_ints, 2, true) == VECT_SEARCH_NOT_FOUND);
  fail_unless(vector_search(v, &num5, compare_ints, 2, true) == 4);

  vector_free(v);
}
END_TEST

START_TEST (search_should_fail_if_invalid_start_parameter)
{

  int num1 = 12, num2 = 22, num3 = 23;

  vector *v = vector_new(sizeof(int *), NULL, 5);
  vector_append(v, &num1);
  vector_append(v, &num2);
  vector_append(v, &num3);

  fail_unless(vector_search(v, &num1, compare_ints, -2, false) == VECT_SEARCH_INVALID_START);
  fail_unless(vector_search(v, &num2, compare_ints, 3, false) == VECT_SEARCH_INVALID_START);
  fail_unless(vector_search(v, &num1, compare_ints, -2, true) == VECT_SEARCH_INVALID_START);
  fail_unless(vector_search(v, &num2, compare_ints, 3, true) == VECT_SEARCH_INVALID_START);

  vector_free(v);
}
END_TEST

START_TEST (replace_element_on_specific_position)
{

  char elem1 = 'a', elem2 = 'b', elem3 = 'c';

  vector *v = vector_new(sizeof(char), NULL, 4);
  vector_append(v, &elem1);
  vector_append(v, &elem2);

  fail_unless(vector_replace(v, 1, &elem3) == 0);
  fail_unless(2 == vector_length(v));
  fail_unless(elem1 == *(char *)vector_get(v, 0));
  fail_unless(elem3 == *(char *)vector_get(v, 1));

  vector_free(v);
}
END_TEST

START_TEST (replace_element_should_call_free_function)
{

  char *elem1 = strdup("first");
  char *elem2 = strdup("second");
  char *elem3 = strdup("third");

  vector *v = vector_new(sizeof(char *), free_string, 4);
  vector_append(v, &elem1);
  vector_append(v, &elem2);

  fail_unless(vector_replace(v, 0, &elem3) == VECT_OK);
  fail_unless(elem3 == *(char **)vector_get(v, 0));

  vector_free(v);
}
END_TEST

START_TEST (replace_element_should_fail_if_invalid_position)
{

  int num = 10, num2 = 20;

  vector *v = vector_new(sizeof(int), NULL, 2);
  vector_append(v, &num);

  fail_unless(vector_replace(v, 1, &num2) == VECT_REPLACE_INVALID_POSITION);
  fail_unless(vector_replace(v, -3, &num2) == VECT_REPLACE_INVALID_POSITION);

  vector_free(v);
}
END_TEST

START_TEST (sort_should_sort_the_vector)
{

  int num1 = 2, num2 = 4, num3 = 5, num4 = 12, num5 = 23;

  vector *v = vector_new(sizeof(int), NULL, 5);
  vector_append(v, &num4);
  vector_append(v, &num2);
  vector_append(v, &num1);
  vector_append(v, &num5);
  vector_append(v, &num3);

  vector_sort(v, compare_ints);

  fail_unless(*(int *)vector_get(v, 0) == num1);
  fail_unless(*(int *)vector_get(v, 1) == num2);
  fail_unless(*(int *)vector_get(v, 2) == num3);
  fail_unless(*(int *)vector_get(v, 3) == num4);
  fail_unless(*(int *)vector_get(v, 4) == num5);

  vector_free(v);
}
END_TEST

START_TEST (sort_does_nothing_if_compare_function_is_null)
{

  int num1 = 3, num2 = 10;

  vector *v = vector_new(sizeof(int), NULL, 5);
  vector_append(v, &num2);
  vector_append(v, &num1);

  vector_sort(v, NULL);

  fail_unless(*(int *)vector_get(v, 0) == num2);
  fail_unless(*(int *)vector_get(v, 1) == num1);

  vector_free(v);
}
END_TEST

START_TEST (map_should_call_functionl_for_each_element)
{

  char *name = strdup("igor");

  vector *v = vector_new(sizeof(char *), free_string, 5);

  vector_append(v, &name);
  vector_map(v, upper_string, NULL);

  fail_unless(strcmp("IGOR", *(char **)vector_get(v, 0)) == 0);

  vector_free(v);
}
END_TEST

START_TEST (map_should_call_functionl_for_each_element_passing_auxiliar_data)
{

  char *name = strdup("igor");
  char aux = 'a';

  vector *v = vector_new(sizeof(char *), free_string, 5);

  vector_append(v, &name);
  vector_map(v, upper_string, &aux);

  fail_unless(strcmp("IGOR", *(char **)vector_get(v, 0)) == 0);
  fail_unless('A' == aux);

  vector_free(v);
}
END_TEST

START_TEST (map_does_nothing_if_map_function_is_null)
{

  char *name = strdup("igor");

  vector *v = vector_new(sizeof(char *), free_string, 5);

  vector_append(v, &name);
  vector_map(v, NULL, NULL);

  fail_unless(strcmp("igor", *(char **)vector_get(v, 0)) == 0);

  vector_free(v);
}
END_TEST

START_TEST (delete_element_from_specific_position)
{

  int num1 = 10, num2 = 20;
  size_t initial_alloc_length;

  vector *v = vector_new(sizeof(int), NULL, 2);
  vector_append(v, &num1);
  vector_append(v, &num2);

  initial_alloc_length = v->alloc_length;

  fail_unless(vector_delete(v, 1) == VECT_OK);
  fail_unless(vector_length(v) == 1);
  fail_unless(*(int *)vector_get(v, 0) == num1);
  fail_unless(initial_alloc_length == v->alloc_length);

  vector_free(v);
}
END_TEST

START_TEST (delete_element_should_call_free_function)
{

  char *sport1 = strdup("winsurf");
  char *sport2 = strdup("kitesurf");
  char *sport3 = strdup("motocross");

  vector *v = vector_new(sizeof(char *), free_string, 5);
  vector_append(v, &sport1);
  vector_append(v, &sport2);
  vector_append(v, &sport3);

  fail_unless(vector_delete(v, 2) == VECT_OK);

  vector_free(v);
}
END_TEST

START_TEST (delete_element_should_shift_elements)
{

  char *sport1 = strdup("winsurf");
  char *sport2 = strdup("kitesurf");
  char *sport3 = strdup("motocross");
  char *sport4 = strdup("surf");

  vector *v = vector_new(sizeof(char *), free_string, 5);
  vector_append(v, &sport1);
  vector_append(v, &sport2);
  vector_append(v, &sport3);
  vector_append(v, &sport4);

  fail_unless(vector_delete(v, 1) == VECT_OK);
  fail_unless(vector_length(v) == 3);

  char *elem0 = *(char **)vector_get(v, 0);
  char *elem1 = *(char **)vector_get(v, 1);
  char *elem2 = *(char **)vector_get(v, 2);

  fail_unless(elem0 == sport1, "element on `0' should continue there. found '%s'", elem0);
  fail_unless(elem1 == sport3, "element on `2' should go to `1'. found '%s'", elem1);
  fail_unless(elem2 == sport4, "element on `3' should go to `2'. found '%s'", elem2);

  vector_free(v);
}
END_TEST

START_TEST (delete_element_should_fail_if_invalid_position)
{

  char *sport1 = strdup("winsurf");

  vector *v = vector_new(sizeof(char *), free_string, 5);
  vector_append(v, &sport1);

  fail_unless(vector_delete(v, 2) == VECT_DELETE_INVALID_POSITION);
  fail_unless(vector_delete(v, -2) == VECT_DELETE_INVALID_POSITION);

  vector_free(v);
}
END_TEST

Suite *
vector_suite(void) {
  Suite *s = suite_create("vector");
  TCase *tc_vector = tcase_create("vector");

  tcase_add_test(tc_vector, new_should_fill_struct_with_defaults);
  tcase_add_test(tc_vector, new_should_return_error_if_invalid_elem_size_or_initial);

  tcase_add_test(tc_vector, should_append_and_get_one_element);
  tcase_add_test(tc_vector, should_append_and_get_multiple_elements);
  tcase_add_test(tc_vector, append_should_grown_if_needed);
  tcase_add_test(tc_vector, get_should_fail_if_invalid_index);
  tcase_add_test(tc_vector, should_use_free_function_to_dealloc_elemns);

  tcase_add_test(tc_vector, insert_element_in_the_beginning);
  tcase_add_test(tc_vector, insert_elements_in_all_positions);
  tcase_add_test(tc_vector, insert_fails_if_position_bigger_than_length_or_negative);
  tcase_add_test(tc_vector, insert_should_grow_if_needed);

  tcase_add_test(tc_vector, search_should_return_element_position);
  tcase_add_test(tc_vector, search_should_fail_if_element_not_found);
  tcase_add_test(tc_vector, search_should_fail_if_invalid_key);
  tcase_add_test(tc_vector, search_should_fail_if_invalid_start_parameter);
  tcase_add_test(tc_vector, search_should_ignore_everything_before_start_parameter);

  tcase_add_test(tc_vector, replace_element_on_specific_position);
  tcase_add_test(tc_vector, replace_element_should_call_free_function);
  tcase_add_test(tc_vector, replace_element_should_fail_if_invalid_position);

  tcase_add_test(tc_vector, sort_should_sort_the_vector);
  tcase_add_test(tc_vector, sort_does_nothing_if_compare_function_is_null);

  tcase_add_test(tc_vector, map_should_call_functionl_for_each_element);
  tcase_add_test(tc_vector, map_should_call_functionl_for_each_element_passing_auxiliar_data);
  tcase_add_test(tc_vector, map_does_nothing_if_map_function_is_null);

  tcase_add_test(tc_vector, delete_element_from_specific_position);
  tcase_add_test(tc_vector, delete_element_should_call_free_function);
  tcase_add_test(tc_vector, delete_element_should_shift_elements);
  tcase_add_test(tc_vector, delete_element_should_fail_if_invalid_position);

  suite_add_tcase(s, tc_vector);

  return s;
}

int main(void) {
  int nfailed;
  Suite *s = vector_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  nfailed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
