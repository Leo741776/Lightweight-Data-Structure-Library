#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CAPACITY 100    // The starting size for the dynamic heap array

// Structure for a max-heap using a contiguous array
typedef struct Heap {
    double *data;       // Array storing the heap elements
    size_t size;        // Current number of elements
    size_t capacity;    // Total allocated space
} Heap;

/*
 * Creates an empty heap with default capacity
 */
Heap *h_create() {
    Heap *h = malloc(sizeof(Heap));

    if (!h) {
        printf("Error: Heap allocation failed.\n");
        return NULL;
    }

    h->data = malloc(DEFAULT_CAPACITY * sizeof(double));
    h->size = 0;
    h->capacity = DEFAULT_CAPACITY;

    return h;
}

/*
 * Frees the memory allocated for the heap array and structure
 */
int h_free(Heap *h) {
    if (!h) {
        printf("Error: Heap allocation failed.\n");
        return -1;
    }

    free(h->data);
    free(h);
    return 0;
}

/*
 * Resizes the internal array when capacity is reached
 */
int h_resize(Heap *h) {
    if (!h) {
        printf("Error: Heap is NULL.\n");
        return -1;
    }

    size_t new_capacity = h->capacity * 2;

    double *buffer = realloc(h->data, new_capacity * sizeof(double));

    if (!buffer) {
        printf("Error: Buffer allocation failed.\n");
        return -1;
    }

    h->data = buffer;
    h->capacity = new_capacity;

    return 0;
}

/*
 * Maintains the max-heap property by "sifting down" an element
 * Used after removing the root to restore order
 */
void h_max_heapify(Heap *h, size_t i) {
    if (!h || h->size == 0) {
        printf("Error: Heap is NULL.\n");
        return;
    }

    // Binary tree mapping in an array
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    size_t largest = i;

    // Compare with left child
    if (left < h->size && h->data[left] > h->data[largest]) {
        largest = left;
    }

    // Compare with right child
    if (right < h->size && h->data[right] > h->data[largest]) {
        largest = right;
    }

    // If a child is larger, swap and continue heapifying down
    if (largest != i) {
        double buffer = h->data[i];
        h->data[i] = h->data[largest];
        h->data[largest] = buffer;

        h_max_heapify(h, largest);
    }
}

/*
 * Inserts a new value into the heap
 * Places it at the end and "sifts up" to maintain max-heap property
 */
void h_insert(Heap *h, double value) {
    if (!h) {
        printf("Error: Heap is NULL.\n");
        return;
    }

    if (h->size >= h->capacity) {
        if (h_resize(h) != 0) {
            printf("Error: Could not resize heap.\n");
            return;
        }
    }

    // Place a new value at the first available leaf position
    size_t i = h->size;
    h->data[i] = value;
    h->size++;

    // Bubble up: Compare with parent and swap if larger
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (h->data[i] > h->data[parent]) {
            double buffer = h->data[i];
            h->data[i] = h->data[parent];
            h->data[parent] = buffer;
            i = parent;
        }
        else {
            break;  // Heap property is restored
        }
    }
}

/*
 * Returns the maximum value (root) without removing it
 */
double h_peek(Heap *h) {
    return (h && h->size > 0) ? h->data[0] : NAN;
}

/*
 * Removes and returns the maximum value
 * Replaces the root with the last element and heapifies down
 */
double h_pop_max(Heap *h) {
    if (!h || h->size == 0) {
        printf("Error: Heap is NULL.\n");
        return NAN;
    }

    double root = h->data[0];

    // Replace root with the last element in the array
    h->data[0] = h->data[h->size - 1];
    h->size--;

    // Restore the heap property from the root down
    h_max_heapify(h, 0);

    return root;
}