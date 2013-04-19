/*
 * queue.c
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <common/queue.h>
#include <common/typedefs.h>

/**
 * Initialize the queue
 * @return the new queue
 */
queue* queue_init() {
	queue* nq = kmalloc(sizeof(queue));
	if (nq == NULL ) {
		return (NULL );
	}
	nq->data = NULL;
	nq->previous = NULL;

	return (nq);
}

/**
 * Put element into queue
 * @param queue the queue
 * @param data the element
 */
int32_t queue_put(queue* que, void* data) {
	if (data == NULL ) {
		return (-1);
	}

	queue* ptr;

	if (que->previous == NULL ) {
		que->previous = kmalloc(sizeof(que));
		if (que->previous == NULL ) {
			return (-1);
		}
		que->previous->data = data;
		que->previous->previous = NULL;
	} else {
		ptr = que;
		while (ptr->previous != NULL ) {
			ptr = ptr->previous;
		}
		que->previous = kmalloc(sizeof(que));
		if (que->previous == NULL ) {
			return (-1);
		}
		que->previous->data = data;
		que->previous->previous = NULL;
	}
	return (0);
}

/**
 * Get last element from queue
 * @param queue the queue
 */
void* queue_get(queue* que) {
	queue* ptr;
	void* data = NULL;
	if(que->previous != NULL) {
		ptr = que->previous;
		que->previous = ptr->previous;
		data = ptr->data;
		kfree(ptr);
		return (data);
	} else {
		return (NULL);
	}
}
