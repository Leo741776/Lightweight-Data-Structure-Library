#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_SIZE 100    // The starting size for the dynamic stack array

// Structure for a dynamic array-based stack
typedef struct Stack {
    double *data;       // Pointer to the array holding stack elements
    size_t top;         // Index of the next available slot (also represents current count)
    size_t capacity;    // Total allocated size of the data array
} Stack;

/*
 * Allocates memory for the Stack structure and its internal array
 * Returns the stack pointer or NULL on failure
 */
Stack *s_create() {
    Stack *s = malloc(sizeof(Stack));

    if (!s) {
        printf("Error: Stack allocation failed.\n");
        return NULL;
    }

    s->data = malloc(DEFAULT_SIZE * sizeof(double));

    if (!s->data) {
        printf("Error: Data allocation failed.\n");
        free(s);    // Clean up the struct if the array allocation fails
        return NULL;
    }

    s->top = 0; // Stack starts empty
    s->capacity = DEFAULT_SIZE;

    return s;
}

/*
 * Frees up the internal array and then the Stack structure itself
 */
int s_free(Stack *s) {
    if (!s) {
        printf("Error: Stack is NULL.\n");
        return -1;
    }

    if (s->data) {
        free(s->data);
        s->data = NULL;
    }

    free(s);

    return 0;
}

/*
 * Doubles the capacity of the stack using realloc
 * realloc handles copying the old data to the new location
 */
int s_resize(Stack *s) {
    if (!s) {
        printf("Error: Stack is NULL.\n");
        return -1;
    }

    size_t new_capacity = s->capacity * 2;

    // realloc attempts to resize the existing block or move it if needed
    double *buffer = realloc(s->data, new_capacity * sizeof(double));

    if (buffer == NULL) {
        printf("Error: Buffer allocation failed.\n");
        return -1;
    }

    s->data = buffer;
    s->capacity = new_capacity;

    return 0;
}

/*
 * Pushes a new value onto the top of the stack
 * Automatically resizes if the capacity is reached
 */
int s_push(Stack *s, double value) {
    if (!s) {
        printf("Error: Stack is NULL.\n");
        return -1;
    }

    // Check if the stack is full
    if (s->top >= s->capacity) {
        if (s_resize(s) != 0) {
            printf("Error: Stack overflow.\n");
            return -1;
        }
    }

    // Assign value then increment top (post-increment)
    s->data[s->top++] = value;

    return 0;
}

/*
 * Removes and returns the top value from the stack
 * Returns NAN if the stack is empty
 */
double s_pop(Stack *s) {
    if (!s) {
        printf("Error: Stack is NULL.\n");
        return NAN;
    }

    if (s->top == 0) {
        printf("Error: Stack is already empty.\n");
        return NAN;
    }

    // Decrement top then return the value at that index (pre-decrement)
    return s->data[--s->top];
}

/*
 * Returns the top value without removing it
 */
double s_peek(Stack *s) {
    if (!s) {
        printf("Error: Stack is NULL.\n");
        return NAN;
    }

    if (s->top == 0) {
        printf("Notice: Stack is already empty.\n");
        return NAN;
    }

    // Peek at the last inserted element
    return s->data[s->top - 1];
}

/*
 * Iterate through the stack, printing elements from bottom to top
 */
int s_print(Stack *s) {
    if (s) {
        printf("Values in current stack: [");
        for (size_t i = 0; i < s->top; i++) {
            printf("%.2f", s->data[i]);
            if (i < s->top - 1) {
                printf(", ");
            }
        }
        printf("]\n");
        return 0;
    }

    printf("Error: Unable to print stack.\n");
    return -1;
}
