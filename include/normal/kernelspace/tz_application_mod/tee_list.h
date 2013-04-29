/*
 * tee_list.h
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#ifndef TEE_LIST_H_
#define TEE_LIST_H_

#include <linux/slab.h>

struct __tee_list {
	void* 			 data; /**< Data for this list entry */
	struct __tee_list* next; /**< next list entry */
	struct __tee_list* prev; /**< prev list entry */
};

typedef struct __tee_list tee_list_t;

void tee_list_init(tee_list_t *head, void* data);
int tee_list_add(tee_list_t* head, void* data);
tee_list_t* tee_list_find_data(tee_list_t* head, void* data);
void tee_list_remove(tee_list_t *node);
int tee_list_is_empty(tee_list_t *head);

//----------------------------------------------------------------------
/// \brief Iterates over all node entires in a list.
///
/// \param _pos Current iterator node
/// \param _head Head node of the list to iterate over.
#define tee_list_for_each(_pos,_head)                              \
  for (_pos = (_head)->next; _pos != (_head); _pos = _pos->next)

//----------------------------------------------------------------------
/// \brief Iterates over all node entires in a list (safe against
///   list modificatins)
///
/// \param _pos Current iterator node
/// \param _next Next iterator node.
/// \param _head Head node of the list to iterate over.
#define tee_list_for_each_safe(_pos,_next,_head)                   \
  for (_pos = (_head)->next, _next = _pos->next;                \
       _pos != (_head); _pos = _next, _next = _pos->next)

#endif /* TEE_LIST_H_ */
