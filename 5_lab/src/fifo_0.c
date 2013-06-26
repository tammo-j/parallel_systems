#include "fifo_0.h"

#include <stdlib.h>
#include <assert.h>


void fifo_init(fifo_queue_t *q) {
	q->head = NULL;
	q->tail = NULL;
	omp_init_lock(&q->lock);
}

void fifo_enqueue(fifo_queue_t *q, int data) {
	fifo_node_t *n;

	/* prepare node */
	n = malloc(sizeof(*n));
	n->data = data;
	n->next = NULL;

	omp_set_lock(&q->lock);

	/* enqueue node */
	if (q->tail)
		q->tail->next = n;
	q->tail = n;

	/* let head point to node if queue was empty */
	if (!q->head)
		q->head = q->tail;

	omp_unset_lock(&q->lock);
}

int fifo_dequeue(fifo_queue_t *q) {
	int data;
	fifo_node_t *n;

	omp_set_lock(&q->lock);

	/* check for empty queue */
	if (!q->head) {
		omp_unset_lock(&q->lock);
		return 0;
	}

	/* dequeue head node */
	n = q->head;
	q->head = q->head->next;

	/* don't let tail point to the dequeued node */
	if (!q->head)
		q->tail = NULL;

	omp_unset_lock(&q->lock);

	/* extract data and free node */
	data = n->data;
	free(n);
	assert(data);

	return data;
}

