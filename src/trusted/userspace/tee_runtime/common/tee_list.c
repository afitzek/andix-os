/*
 * tee_list.c
 *
 *  Created on: Mar 4, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <tee_internal_api.h>

/**
 * \defgroup list Common list
 *
 */

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

bool list_add(list* head, uintptr_t data)
{
	list* node = NULL;
	node = (list*) malloc(sizeof(list));
	if(node == NULL) {
		return (false);
	}
	node->data = data;
	list_link(head->prev, head, node);
	return (true);
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
  free((uintptr_t)node);
}

int list_is_empty(list *head)
{
  return (head == head->next);
}
