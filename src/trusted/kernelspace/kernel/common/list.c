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
 * list.c
 *
 *  Created on: Nov 11, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/list.h>
#include <mm/mm.h>

void list_init(list *head, uintptr_t data) {
	head->next = head;
	head->prev = head;
	head->data = data;
}

void list_link(list* prev, list* next, list* node)
{
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

void list_dump(list* head) {
	list_debug("List: 0x%x", head);
	list_debug("Next: 0x%x", head->next);
	list_debug("Prev: 0x%x", head->prev);
	list_debug("Data: 0x%x", head->data);
}

uint8_t list_add(list* head, uintptr_t data)
{
	list* node = NULL;
	node = (list*) kmalloc(sizeof(list));
	if(node == NULL) {
		list_error("Failed to allocate memory for list node!");
		return (-1);
	}
	node->data = data;
	list_link(head->prev, head, node);
	return (0);
}

list* list_find_data(list* head, uintptr_t data) {
	list* pos;
	list* next;
	list_for_each_safe(pos, next, head) {
		if(pos->data == data) {
			return (pos);
		}
	}
	return (NULL);
}

void list_remove(list *node)
{
  list *next = node->next;
  list *prev = node->prev;

  next->prev = prev;
  prev->next = next;
  node->next = node;
  node->prev = node;
  kfree((uintptr_t)node);
}

int list_is_empty(list *head)
{
  return (head == head->next);
}
