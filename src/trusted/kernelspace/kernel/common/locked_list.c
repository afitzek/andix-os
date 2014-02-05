/**
 * @file locked_list.c
 * @brief
 *  Created on: Jan 17, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */

#include <common/typedefs.h>
#include <common/locked_list.h>
#include <mm/mm.h>


static void link(struct list_element *prev, struct list_element *next, struct list_element *node)
{
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

static void unlink(struct list_element *node) {
	struct list_element *next = node->next;
	struct list_element *prev = node->prev;

	next->prev = prev;
	prev->next = next;
	node->next = node;
	node->prev = node;
}

static struct list_element *find_data(struct list_element *head, uintptr_t data) {
	struct list_element *pos;
	list_for_each(pos, head) {
		if(pos->data == data)
			return (pos);
	}
	return NULL;
}

void locked_list_init(locked_list *ll) {
	mutex_init(&(ll->mutex));
	ll->head = NULL;
}

int locked_list_is_empty(locked_list *ll) {
	return ll->head == NULL;
}

struct list_element *locked_list_add(locked_list *ll, void *data) {
	struct list_element *node;
	node = (struct list_element*) kmalloc(sizeof(list));
	if(node == NULL) {
		list_error("Failed to allocate memory for list node!");
		return 0;
	}
	node->data = data;
	mutex_lock(&(ll->mutex));
	if (locked_list_is_empty(ll)) {
		node->next = node->prev = node;
		ll->head = node;
	}
	link(ll->head->prev, ll->head, node);
	mutex_unlock(&(ll->mutex));
	return 1;
}

static void remove(locked_list *ll, struct list_element *node) {
	if (node == ll->head) {
		if (ll->head->next == ll->head)
			ll->head = NULL;
		else
			ll->head = ll->head->next;
	}
	unlink(node);
}

void locked_list_remove(locked_list *ll, struct list_element *node) {
	mutex_lock(&(ll->mutex));
	remove(ll, node);
	mutex_unlock(&(ll->mutex));
	kfree(node);
}

struct list_element *locked_list_find_data(locked_list *ll, void *data) {
	struct list_element *match;
	mutex_lock(&(ll->mutex));
	match = find_data(ll->head, data);
	mutex_unlock(&(ll->mutex));
	return match;
}

int locked_list_remove_match(locked_list *ll, void *data) {
	struct list_element *match;
	mutex_lock(&(ll->mutex));
	match = find_data(ll->head, data);
	if (match)
		remove(ll, match);
	mutex_unlock(&(ll->mutex));
	return match != NULL;
}

int locked_list_for_each(locked_list *ll, int (*func)(void *list_data, void *func_data), void *func_data) {
	mutex_lock(&(ll->mutex));
	for (struct list_element *p = ll->head; p; p = p->next) {
		int ret = func(p->data, func_data);
		if  (ret) {
			mutex_unlock(&(ll->mutex));
			return ret;
		}
		if (p == ll->head->prev)
			break;
	}
	mutex_unlock(&(ll->mutex));
	return 0;
}

void locked_list_iter_init(struct locked_list_iterator *iter, struct locked_list_t *ll) {
	iter->ll = ll;
	iter->nxt = ll->head;
	iter->wrapped = 0;
}

void *locked_list_iter_next(struct locked_list_iterator *iter) {
	if (iter->nxt == NULL)	// empty or broken list
		return NULL;
	if (iter->wrapped)
		return NULL;
	void *d = iter->nxt->data;
	iter->nxt = iter->nxt->next;
	if (iter->nxt == iter->ll->head)	// last element
		iter->wrapped = 1;
	return d;
}
