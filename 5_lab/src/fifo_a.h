#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>
#include <omp.h>

typedef struct fifo_node {
	struct fifo_node *next;
	int data;
	bool valid;
} fifo_node_t;

typedef struct fifo_queue {
	omp_lock_t head_lock;
	omp_lock_t tail_lock;
	fifo_node_t *head;
	fifo_node_t *tail;
} fifo_queue_t;

/* inits the fifo queue */
void fifo_init(fifo_queue_t *q);
/* frees fifo queue (not thread secure) */
void fifo_free(fifo_queue_t *q);
/* appends data */
void fifo_enqueue(fifo_queue_t *q, int data);
/* returns head's data */
int fifo_dequeue(fifo_queue_t *q);
/* returns if queue is empty */
bool fifo_empty(fifo_queue_t *q);

#endif // FIFO_H
