/*
 * tee_list.h
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef TEE_LIST_H_
#define TEE_LIST_H_


/**
 * \addtogroup list
 *	\{
 */


/**
 *	Generic list type
 */
struct list_t {
	uintptr_t			data; /**< Data for this list entry */
	struct list_t*		next; /**< next list entry */
	struct list_t*		prev; /**< prev list entry */
};

/**
 * The list type
 */
typedef struct list_t list;

/**
 * Initialize the list
 *
 * @param head Head of the list
 * @param data Data of the list
 */
void list_init(list *head, uintptr_t data);

/**
 * Link node to list
 *
 * @param prev The prev list node
 * @param next The next list node
 * @param node The node to link
 */
void list_link(list* prev, list* next, list* node);

/**
 * Add data to the list
 *
 * @param head The list head
 * @param data The data to link
 * @return true on success, false on error
 */
bool list_add(list* head, uintptr_t data);

/**
 * Remove node from list
 *
 * @param node The node to remove
 */
void list_remove(list *node);

/**
 * Checks if list is empty
 *
 * @param head The list head
 * @return list empty
 */
int list_is_empty(list *head);

/**
 * Find list node with data in list
 *
 * @param head The list head
 * @param data The list data
 * @return the list node with list data
 */
list* list_find_data(list* head, uintptr_t data);

//----------------------------------------------------------------------
/// \brief Iterates over all node entires in a list.
///
/// \param _pos Current iterator node
/// \param _head Head node of the list to iterate over.
#define list_for_each(_pos,_head)                              \
  for (_pos = (_head)->next; _pos != (_head); _pos = _pos->next)

//----------------------------------------------------------------------
/// \brief Iterates over all node entires in a list (safe against
///   list modificatins)
///
/// \param _pos Current iterator node
/// \param _next Next iterator node.
/// \param _head Head node of the list to iterate over.
#define list_for_each_safe(_pos,_next,_head)                   \
  for (_pos = (_head)->next, _next = _pos->next;                \
       _pos != (_head); _pos = _next, _next = _pos->next)


/* \} group */

#endif /* TEE_LIST_H_ */
