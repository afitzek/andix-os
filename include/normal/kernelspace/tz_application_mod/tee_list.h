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
