#ifndef _FIFO_H_
#define _FIFO_H_

#include <error.h>
#include <stdbool.h>
#include <stdlib.h>

struct fifo {
    int *head;      // where to read from?
    int *tail;      // where to write?
    int size;       // number of elements
    int elements[]; // actual array
};

// create a FIFO of given size
struct fifo *fifo_create(int size) {
    struct fifo *queue = malloc(sizeof(struct fifo) + size * sizeof(int));
    queue->size = size;
    queue->head = queue->tail = queue->elements;
    return queue;
}

// destroy a FIFO (release memory)
void fifo_destroy(struct fifo *queue) {
    free(queue);
}

// return true if FIFO is empty, false otherwise
bool fifo_is_empty(struct fifo *queue) {
    return queue->head >= queue->tail;
}

// add element to FIFO
void fifo_enqueue(struct fifo *queue, int value) {
    if (queue->tail - queue->elements >= queue->size) {
        error(EXIT_FAILURE, 0, "Exceeded FIFO queue size");
    }

    *queue->tail = value;
    queue->tail++;
}

// remove element from FIFO
int fifo_dequeue(struct fifo *queue) {
    if (fifo_is_empty(queue)) {
        error(EXIT_FAILURE, 0, "Empty FIFO queue");
    }

    return *(queue->head++);
}

#endif
