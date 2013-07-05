/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
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
