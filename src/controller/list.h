#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

/** @struct list_s
 *  @brief This structure implements a list
 *  @var list_s::head
 *  Member 'head': The head of the list
 *  @var list_s::copy
 *  Member 'copy': contains the function used for copy
 *  @var list_s::cmp
 *  Member 'cmp': contains the function used for compare
 *  @var list_s::del
 *  Member 'del': contains the function used for delete
 */
typedef struct list_s {
    struct element_s *head;

    void *(*copy)(void *);
    int (*cmp)(void *, void *);
    void (*del)(void *);
} list_t;

/** @struct element_s
 *  @brief This structure defines an element
 *  @var element_s::v
 *  Member 'v': Element content
 *  @var element_s::next
 *  Member 'next': Next element
 */
struct element_s {
    void *v;
    struct element_s *next;
};

/*!
 * \fn list_empty(void *(*copy)(void *), int (*cmp)(void *, void *), void (*del)(void *))
 * \brief Creates and returns an empty list, while defining the copying, comparing and deleting functions
 * \param copy Copying element function
 * \param cmp Comparing element function
 * \param del Deleting element function
 */
list_t *list_empty(void *(*copy)(void *), int (*cmp)(void *, void *), void (*del)(void *));

/*!
 * \fn list_add(list_t *list, void *v)
 * \brief Adding element v to list (v can be a fish or view depending on the use)
 * \param list List
 * \param v Element
 */
int list_add(list_t *list, void *v);

/*!
 * \fn list_del(list_t *list, void *v)
 * \brief Deleting element v from list (v can be a fish or view depending on the use)
 * \param list List
 * \param v Element
 */
int list_del(list_t *list, void *v);

/*!
 * \fn list_is_member(list_t *list, void *v)
 * \brief Checks if v is member of list (v can be a fish or view depending on the use)
 * \param list List
 * \param v Element
 */
int list_is_member(list_t *list, void *v);

/*!
 * \fn list_at(list_t *list, int i)
 * \brief Returns the element which is at index i of list (This element can be a fish or view depending on the use)
 * \param list List
 * \param i Index
 */
void *list_at(list_t *list, int i);

/*!
 * \fn list_size(list_t *list)
 * \brief Returns the size of list
 * \param list List
 */
size_t list_size(list_t *list);

/*!
 * \fn list_is_empty(list_t *list)
 * \brief Checks if list is empty
 * \param list List
 */
int list_is_empty(list_t *list);

/*!
 * \fn list_free(list_t *list)
 * \brief Frees the list and its elements, if not freed already
 * \param list List
 */
void list_free(list_t *list);

/*!
 * \fn list_debug(list_t *list)
 * \brief Prints elements of list, used to debug, should be used carefully as list could have fishes or views 
 * \param list List
 */
void list_debug(list_t *list);

#endif