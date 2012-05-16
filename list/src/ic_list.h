
#include <stdbool.h>
#include <stddef.h>

#ifndef _ICLIB_LIST
#define _ICLIB_LIST

/**
 * List and Node types
 *
 * You should *not* access any element of these structs directly
 *
 * Note that each ic_node (list item) stores a pointer to the
 * element you added to the list. Your data is not copied to the
 * list, it stores just a pointer to your data.
 * You need to make sure this data remais valid while the list is
 * in use.
 *
 */
typedef struct ic_node {
  struct ic_node *prev;
  struct ic_node *next;
  void *data;
} ic_node;

typedef struct {
  ic_node *head;
  ic_node *tail;
  size_t length;
} ic_list;


/**
 * Allocates a new list and returns a pointer to it.
 *
 * You must call ic_list_free() when done
 */
ic_list * ic_list_new(void);

/**
 * Returns true if the list is empty, false otherwise
 */
bool ic_list_empty(ic_list *);

/**
 * Returns the number of elements in the list
 */
size_t ic_list_length(ic_list *l);

/**
 * Add one element to the tail of the list
 */
void ic_list_append(ic_list *l, void *data);

/**
 * Add one element to the head of the list
 */
void ic_list_prepend(ic_list *l, void *data);

/**
 * Return element from head of the list. NULL if list is empty
 */
void * ic_list_first(ic_list *l);

/**
 * Return element from tail of the list. NULL if list is empty
 */
void * ic_list_last(ic_list *l);

/**
 * Returns the element at the given position, NULL if the position is off the end
 */
ic_node * ic_list_nth(ic_list *l, size_t n);

/**
 * Returns the data from element at the given position, NULL if the position
 * is off the end
 */
void * ic_list_nth_data(ic_list *l, size_t n);

/**
 * Frees all elements from the list
 */
void ic_list_free(ic_list *l);


#endif
