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
 * tee_list.c
 *
 *  Created on: Apr 28, 2013
 *      Author: andy
 */

#include <tz_application_mod/tee_list.h>

void tee_list_init(tee_list_t *head, void* data) {
	head->next = head;
	head->prev = head;
	head->data = data;
}

void tee_list_link(tee_list_t* prev, tee_list_t* next, tee_list_t* node) {
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

int tee_list_add(tee_list_t* head, void* data) {
	tee_list_t* node = NULL;
	node = (tee_list_t*) kmalloc(sizeof(tee_list_t), GFP_KERNEL);
	if (node == NULL) {
		printk(KERN_ERR "Failed to allocate memory for list node!");
		return (-1);
	}
	node->data = data;
	tee_list_link(head->prev, head, node);
	return (0);
}

tee_list_t* tee_list_find_data(tee_list_t* head, void* data) {
	tee_list_t* pos;
	tee_list_t* next;
	tee_list_for_each_safe(pos, next, head)
	{
		if (pos->data == data) {
			return (pos);
		}
	}
	return (NULL);
}

void tee_list_remove(tee_list_t *node) {
	tee_list_t *next = node->next;
	tee_list_t *prev = node->prev;

	next->prev = prev;
	prev->next = next;
	node->next = node;
	node->prev = node;
	kfree((void*) node);
}

int tee_list_is_empty(tee_list_t *head) {
	return (head == head->next);
}
