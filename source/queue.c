#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DEFAULT_SIZE 100    // The starting size for the dynamic queue array

// Structure to represent a circular dynamic queue
typedef struct Queue {
    double *data; // Pointer to the array of elements
    size_t head; // Index of the first element
    size_t tail; // Index of the last element
    size_t capacity; // Max number of elements currently possible
    size_t size; // Current number of elements in the queue
} Queue;

/*
 * Initializes a new queue with default capacity
 * Returns a pointer to the queue or NULL on failure
 */
Queue *q_create() {
    Queue *q = malloc(sizeof(Queue));

    if (!q) {
        return NULL;
    }

    q->data = malloc(DEFAULT_SIZE * sizeof(double));

    if (!q->data) {
        free(q); // Clean up the struct if the data array fails
        return NULL;
    }

    // Initialize state for an empty queue starting at index 0
    q->head = 0;
    q->tail = 0;
    q->capacity = DEFAULT_SIZE;
    q->size = 0;

    return q;
}

/*
 * Frees all the memory associated with the queue
 */
int q_free(Queue *q) {
    if (!q) {
        return -1;
    }

    if (q->data) {
        free(q->data);
        q->data = NULL;
    }

    free(q);

    return 0;
}

/*
 * Doubles the capacity of the queue when full
 * Realigns elements to start from index 0 in the new buffer
 */
int q_resize(Queue *q) {
    if (!q) {
        return -1;
    }

    size_t old_capacity = q->capacity;
    size_t new_capacity = q->capacity * 2;

    double *buffer = malloc(new_capacity * sizeof(double));

    if (!buffer) {
        return -1;
    }

    // Realign data, map [head...end] and [0...tail] to [0...size] in new buffer
    for (size_t i = 0; i < q->size; i++) {
        buffer[i] = q->data[(q->head + i) % old_capacity];
    }

    free(q->data); // Release old memory
    q->data = buffer;
    q->head = 0; // Reset head to start of new buffer
    q->tail = q->size; // Tail points to the first empty slot
    q->capacity = new_capacity;

    return 0;
}

/*
 * Adds an element to the back of the queue
 * Resizes if the queue array is full
 */
int q_enqueue(Queue *q, double value) {
    if (!q) {
        return -1;
    }

    // Check if resize is needed
    if (q->size == q->capacity) {
        if (q_resize(q) != 0) {
            return -1;
        }
    }

    // Insert at tail and wrap around using modulo if needed
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;

    return 0;
}

/*
 * Removes and returns the element at the front of the queue
 * Returns NAN if the queue is empty
 */
double q_dequeue(Queue *q) {
    if (!q) {
        return NAN;
    }

    if (q->size == 0) {
        return NAN;
    }

    double dequeued_value = q->data[q->head];

    // Move head forward and wrap around using modulo
    q->head = (q->head + 1) % q->capacity;
    q->size--;

    return dequeued_value;
}

/*
 * Returns the front element without removing it
 */
double q_peek(Queue *q) {
    if (!q) {
        return NAN;
    }

    if (q->size == 0) {
        return NAN;
    }

    return q->data[q->head];
}

/*
 * Prints all elements currently in the queue
 */
int q_print(Queue *q) {
    if (!q) {
        return -1;
    }

    printf("Queue: [");
    for (size_t i = 0; i < q->size; i++) {
        // Calculate the actual index in the circular buffer
        size_t index = (q->head + i) % q->capacity;
        printf("%.2f%s", q->data[index], (i < q->size - 1) ? ", " : "");
    }
    printf("]\n");

    return 0;
}
