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
