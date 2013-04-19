/*
 * queue.h
 *
 *  Created on: Jan 18, 2013
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <common/typedefs.h>

/**
 * The queue structure
 */
struct queue_t {
	void* data;
	struct queue_t* previous;
};

/**
 * The queue typedef
 */
typedef struct queue_t queue;

/**
 * Initialize the queue
 * @return the new queue
 */
queue* queue_init();

/**
 * Put element into queue
 * @param queue the queue
 * @param data the element
 * @return if != 0 failed
 */
int32_t queue_put(queue* queue, void* data);

/**
 * Get last element from queue
 * @param queue the queue
 */
void* queue_get(queue* queue);

#endif /* QUEUE_H_ */
