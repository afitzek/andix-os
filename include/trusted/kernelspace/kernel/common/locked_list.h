/**
 * @file locked_list.h
 * @brief
 *  Created on: Jan 17, 2014
 * @author Florian Achleitner <florian.achleitner@student.tugraz.at>
 */

#ifndef LOCKED_LIST_H_
#define LOCKED_LIST_H_

#include <lock.h>


struct list_element {
	struct list_element *prev, *next;
	void *data;
};

struct locked_list_t {
	mutex_t mutex;
	struct list_element *head;
};

typedef struct locked_list_t locked_list;

void locked_list_init(locked_list *ll);

int locked_list_is_empty(locked_list *ll);

struct list_element *locked_list_add(locked_list *ll, void *data);

void locked_list_remove(locked_list *ll, struct list_element *node);

struct list_element* locked_list_find_data(locked_list *ll, void *data);

int locked_list_remove_match(locked_list *ll, void *data);

int locked_list_for_each(locked_list *ll, int (*func)(void *list_data, void *func_data), void *func_data);


struct locked_list_iterator {
	struct locked_list_t *ll;
	struct list_element *nxt;
	int wrapped;
};

void locked_list_iter_init(struct locked_list_iterator *iter, struct locked_list_t *ll);
void *locked_list_iter_next(struct locked_list_iterator *iter);


#endif /* LOCKED_LIST_H_ */
