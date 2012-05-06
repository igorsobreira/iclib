#include <stdlib.h>
#include <stdbool.h>

/**
 * Vector
 *
 * The vector allows the client to store any number of elements of any desired
 * primitive types.
 * It supports efficient access and appending/inserting/deleting elements as well
 * as searching and optional sorting.
 */

#ifndef _VECTOR
#define _VECTOR

/**
 * Possible error conditions from various vector functions. Each function documents
 * what are the possible errors it could raise
 */
enum {
  VECT_OK = 0,
  VECT_SEARCH_NOT_FOUND = -1,
  VECT_SEARCH_INVALID_KEY = -2,
  VECT_SEARCH_INVALID_START = -3,
  VECT_INSERT_INVALID_POSITION = -4,
  VECT_REPLACE_INVALID_POSITION = -5,
  VECT_DELETE_INVALID_POSITION = -6,
};

/**
 * Type: vector_cmp_func
 *
 * ``vector_cmp_func`` is a pointer to a client-supplied function which the
 * vector uses to sort or search for elements. The comparator takes two
 * (const void *) pointers (these will point to elements) and returns an int.
 * The comparator should indicate the ordering of the two elements
 * using the same convention as the ``strcmp`` library function:
 *
 *   If elem_ptr1 is less than elem_ptr2, return a negative number.
 *   If elem_ptr1 is greater than elem_ptr2, return a positive number.
 *   If the two elements are equal, return 0.
 */
typedef int (*vector_cmp_func)(const void *elem_ptr1, const void *elem_ptr2);


/**
 * Type: vector_map_func
 *
 * ``vector_map_func`` is a pointer to a client-supplied function that can be
 * used to map over the elements in a vector. A map function is called with
 * a pointer to the element and a client data pointer passed in from the original
 * caller.
 */
typedef void (*vector_map_func)(void *elem_ptr, void *aux_data);


/**
 * Type: vector_free_func
 *
 * ``vector_free_func`` is a pointer to client-supplied function used to
 * clean-up each element when it's removed from the vector, or when the
 * vector is vector is destroyed
 */
typedef void (*vector_free_func)(void *elem_ptr);


/**
 * Type: vector
 *
 * Defines the concrete representation of the vector.
 * This type should not be accessed directly, all the fields are private. The
 * client should interact using the functions defined bellow.
 */
typedef struct {
  void *elems;
  size_t elem_size;
  int step;
  size_t length;
  size_t alloc_length;
  vector_free_func free_func;
} vector;


/**
 * Function: vector_new
 * Usage: vector *my_friends = vector_new(sizeof(char *), string_free, 10);
 *
 * Constructs a raw or previously destroyed vector to be the empty vector.
 *
 * Parameters
 *
 * ``elem_size``
 *   specifies the number of bytes that a single element of the vector should take up.
 *   For example, if you want to store elements of type char *, you would pass
 *   sizeof(char *) as this parameter.

 * ``vector_free_func``
 *   function that will be called on an element that is about to be deleted
 *   (using ``vector_delete``) or on each element in the vector when the entire
 *   vector is being freed (using ``vector_free``). This function.
 *   Should be NULL  if the elements don't require any special handling.
 *
 * ``initial``
 *   specifies the initial allocated length of the vector, as well as the dynamic
 *   reallocation increment for those times when the vector needs to grow.
 *   Rather than growing the vector one element at a time as elements are added,
 *   you will grow the vector in chunks of ``initial`` size.
 *
 * The allocated length is the number of elements for which space has been
 * allocated: the logical length is the number of those slots currently being used.
 *
 * A new vector pre-allocates space for ``initial`` elements, but the
 * logical length is zero. As elements are added, those allocated slots fill
 * up, and when the initial allocation is all used, grow the vector by another
 * ``initial`` elements. You will continue growing the vector in chunks
 * like this as needed. Thus the allocated length will always be a multiple
 * of ``initial``.
 *
 * No realloc is performed to shrink the vector if elements are deleted.
 *
 * Returns
 *
 *   a vector * on success
 *   NULL if ``elem_size`` or ``initial`` are 0 (zero)
 *
 * Note that the call to ``vector_free`` is mandatory
 *
 */
vector* vector_new(size_t elem_size, vector_free_func free_func, int initial);

/**
 * Function: vector_length
 *
 * Returns
 *
 *  The number of elements currenly in the vector (logical length).
 *
 * Complexity: O(1)
 *
 */
size_t vector_length(const vector *v);

/**
 * Function: vector_append
 *
 * Add element to the end of the vector.
 * The logical length is incremented by one. If the allocated length is increased
 * (see ``vector_new`` docs) if necessary.
 *
 * Parameters
 *
 *  ``elem_ptr``
 *    a pointer to the element to be appended. The pointer contents are copied to
 *    the last position of the vector.
 *
 * Complexity: O(1), ignoring the grown if necessary
 *
 */
void vector_append(vector *v, const void *elem_ptr);

/**
 * Function: vector_insert
 *
 * Inserts an element at the specified position.
 *
 * Parameters
 *
 *  ``elem_ptr``
 *    a pointer to the element to be inserted. The pointer contents are copied to
 *    the specified ``position``.
 *  ``position``
 *    index where the new element will be inserted
 *
 * The vector elements after the supplied position will be shifted over to make room.
 *
 * Returns
 *
 *  VECT_OK on success
 *  VECT_INSERT_INVALID_POSITION if ``position`` is greater than logical length
 *    or less than zero
 *
 * Complexity: O(1)
 *
 */
int vector_insert(vector *v, const void *elem_ptr, int position);

/**
 * Function: vector_search
 *
 * Searches the specified vector for an element whose contents match
 * the element passed as the key.
 *
 * Parameters
 *
 *   ``key``
 *     pointer to element to be searched for
 *
 *   ``cmp_func``
 *     function used to test arguments for equality
 *
 *   ``start``
 *     controls where the search starts. Will search from there to the
 *     end of the vector.
 *
 *   ``is_sorted``
 *     if true a faster binary search is performed. if false it uses a
 *     linear search
 *
 * Returns
 *
 *   The position of the matching element if found.
 *   VECT_SEARCH_NOT_FOUND if element was not found
 *   VECT_SEARCH_INVALID_KEY if ``key`` is NULL
 *   VECT_SEARCH_INVALID_START if ``start`` is
 *
 * Complexity: O(log n) if ``is_sorted`` is true. O(n) if ``is_sorted`` is false.
 *
 */
int vector_search(const vector *v, const void *key, vector_cmp_func cmp_func,
                  int start, bool is_sorted);

/**
 * Function: vector_get
 *
 * Returns a pointer to the element on ``position``. First position is 0.
 *
 * Note this function returns a pointer into the vector's storage, so the
 * pointer should be used with care.
 * The a pointer returned by ``vector_get`` becomes invalid after any calls
 * which involve insertion into, deletion from or sorting of the vector, as
 * all of these may rearrange the elements to some extent.
 *
 * Parameters
 *
 *  ``position``
 *    the index of the element to be returned
 *
 * Returns
 *
 *  A pointer to the element on ``position``.
 *  NULL if position is < 0 or greater than logical length
 *
 * Complexity: O(1)
 *
 */
void *vector_get(const vector *v, int position);

/**
 * Function: vector_replace
 *
 * Overwrites the element at the specified position with a new value.

 * Before being overwritten, the ``vector_free_func`` that was supplied
 *  to ``vector_new`` is called for the old element. Then that position
 * in the vector will get a new value by copying the new element's contents
 * from the memory pointed to by ``elem_ptr``.
 *
 * None of the other elements are affected or rearranged by this operation,
 * and the size of the vector remains constant.
 *
 * Parameters
 *
 *   ``position``
 *     index of the element to be replaced
 *
 *  ``elem_ptr``
 *    pointer to the new element that will be copied to ``position``
 *
 * Returns
 *
 *   VECT_OK on success
 *   VECT_REPLACE_INVALID_POSITION if ``position`` is < 0 or greater than logical length
 *
 * Complexity: O(1)
 *
 */
int vector_replace(vector *v, int position, const void *elem_ptr);

/**
 * Function: vector_sort
 *
 * Sorts the vector into ascending order according to the supplied comparator.
 * The algorithm used is quicksort (qsort from stdlib.h)
 *
 * If ``cmp_func`` is NULL nothing is done
 *
 */
void vector_sort(vector *v, vector_cmp_func cmp_func);

/**
 * Function: vector_map
 *
 * Iterate over the elements of the vector in order and calls a ``vector_map_function``
 * passing each element as parameter.
 *
 * Parameters
 *
 *  ``map_func``
 *    this function will be called for each element passing the element and ``data``
 *    parameters. If this parameter is NULL, does the function does nothing.
 *
 *  ``data``
 *    auxiliar data the client can pass to each ``map_func``
 *
 * Complexity: O(n)
 *
 */
void vector_map(vector *v, vector_map_func map_func, void *data);

/**
 * Function: vector_delete
 *
 * Deletes the element at the specified position from the vector.
 *
 * Before the element is removed, the ``vector_free_func`` that was supplied
 * to vector_new will be called on the element.
 *
 * All the elements after the specified position will be shifted over to
 * fill the gap. It does not shrink the allocated size.
 *
 * Parameters
 *
 *   ``position``
 *     index of the element to be removed
 *
 * Returns
 *
 *   VECT_OK on success
 *   VECT_DELETE_INVALID_POSITION if ``position`` is < 0 or greater than logical length
 *
 * Complexity: O(1)
 *
 */
int vector_delete(vector *v, int position);

/**
 * Function: vector_free
 *
 * Frees up all the memory of the specified vector and its elements.
 * ``vector_free_func`` will be called for all elements in the vector.
 *
 * Complexity: O(n), if ``vector_free_func`` is NULL then it's O(1)
 *
 */
void vector_free(vector *v);

#endif
