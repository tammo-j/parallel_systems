#include "fifo_b.h"

#include <stdlib.h>
#include <assert.h>


#define CAS(PTR, OLDVAL, NEWVAL) __sync_bool_compare_and_swap(PTR, OLDVAL, NEWVAL)

static void fifo_move_tail(fifo_queue_t *q);

void fifo_init(fifo_queue_t *q) {
	fifo_node_t *h;
	
	h = malloc(sizeof(*h));
	h->next = NULL;
	
	q->ghost = h;
	q->head = h;
	q->tail = h;
}

void fifo_free(fifo_queue_t *q) {
	fifo_node_t *cur, *next;
	
	// for all nodes 
	for (cur = q->ghost; cur; cur = next) {
		next = cur->next;
		free(cur);
	}
}

void fifo_free_ghosts(fifo_queue_t *q) {
	fifo_node_t *cur, *next;
	
	// until the last ghost is freed 
	for (cur = q->ghost; cur != q->head; cur = next) {
		next = cur->next;
		free(cur);
	}
	
	q->ghost = q->head;
}

void fifo_move_tail(fifo_queue_t *q) {
	fifo_node_t *t = q->tail;
	
	// while not the real tail
	while (t->next) {
		// if not modified by others
		// move tail forward
		CAS(&q->tail, t, t->next);
		
		// use moved tail
		// note that it could be modified by others
		// after the CAS
		t = q->tail;
	}
}

void fifo_enqueue(fifo_queue_t *q, int data) {
	fifo_node_t *t, *n;

	// prepare new node
	n = malloc(sizeof(*t));
	n->data = data;
	n->next = NULL;
	
	// enqueue node
	
	t = q->tail;
	// while modified by others
	while (!CAS(&t->next, NULL, n)) {
		// try to move the tail (help incomplete thread)
		fifo_move_tail(q);
		t = q->tail;
	}
	
	// try to move the tail (may already be moved by others)
	fifo_move_tail(q);
}

int fifo_dequeue(fifo_queue_t *q) {
	fifo_node_t *h, *n;
	
	do {
		h = q->head;
		n = h->next;
		
		// return if queue is empty
		if (!n) return -1;
	} while (!CAS(&q->head, h, n));
	
	return n->data;
}

bool fifo_empty(fifo_queue_t *q) {
	return q->head->next == NULL;
}

