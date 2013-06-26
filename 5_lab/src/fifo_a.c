#include "fifo_a.h"

#include <stdlib.h>
#include <assert.h>


void fifo_init(fifo_queue_t *q) {
	fifo_node_t *h, *t;
	
	h = malloc(sizeof(*h));
	t = malloc(sizeof(*t));
	h->valid = false;
	t->valid = false;
	h->next = t;
	t->next = NULL;
	
	q->head = h;
	q->tail = t;
	omp_init_lock(&q->head_lock);
	omp_init_lock(&q->tail_lock);
}

void fifo_free(fifo_queue_t *q) {
	fifo_node_t *cur, *next = q->head;
	
	while (cur) {
		next = cur->next;
		free(cur);
		cur = next;
	}
	
	omp_destroy_lock(&q->head_lock);
	omp_destroy_lock(&q->tail_lock);
}

void fifo_enqueue(fifo_queue_t *q, int data) {
	fifo_node_t *t, *n;

	// prepare dummy node
	t = malloc(sizeof(*t));
	t->valid = false;
	t->next = NULL;

	omp_set_lock(&q->tail_lock);
	
	// enqueue node
	n = q->tail;
	n->data = data;
	// TODO don't swap valid and data
	n->valid = true;
	n->next = t;
	
	q->tail = t;

	omp_unset_lock(&q->tail_lock);
}

int fifo_dequeue(fifo_queue_t *q) {
	int data;
	fifo_node_t *h, *n;

	omp_set_lock(&q->head_lock);
	
	h = q->head;
	n = h->next;
	
	// check for empty queue
	if (n->valid) {
		// dequeue head node
		data = n->data;
		n->valid = false;
		q->head = n;
		free(h);
	} else {
		// do nothing
		data = -1;
	}

	omp_unset_lock(&q->head_lock);

	return data;
}

bool fifo_empty(fifo_queue_t *q) {
	bool res;
	
	omp_set_lock(&q->head_lock);
	omp_set_lock(&q->tail_lock);
	
	res = q->head->next == q->tail;
	
	omp_unset_lock(&q->tail_lock);
	omp_unset_lock(&q->head_lock);
	
	return res;
}

