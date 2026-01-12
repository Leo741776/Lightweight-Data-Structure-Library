#include <stdio.h>
#include <stdlib.h>

// Represents a single link in the list
typedef struct Node {
    double data;
    struct Node *next;  // Pointer to node coming after
} Node;

// Wrapper structure to manage the entire list
typedef struct LinkedList {
    Node *head;     // Pointer to the first node
    Node *tail;     // Pointer to the last node
    size_t size;    // Total number of nodes
} LinkedList;

/*
 * Allocates and initializes an empty linked list
 */
LinkedList *ll_create() {
    LinkedList *ll = malloc(sizeof(LinkedList));

    if (!ll) {
        printf("Error: Linked list allocation failed.\n");
        return NULL;
    }

    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;

    return ll;
}

/*
 * Iterates through every node to free memory, then frees the list wrapper
 */
int ll_free(LinkedList *ll) {
    if (!ll) {
        printf("Error: Linked list is NULL.\n");
        return -1;
    }

    Node *current_node = ll->head;
    Node *next_node;

    while (current_node != NULL) {
        next_node = current_node->next; // Save the next pointer before freeing
        free(current_node);
        current_node = next_node;
    }

    free(ll);

    return 0;
}

/*
 * Returns a pointer t the node at a specific index
 * Note: This is an O(n) operation as it must traverse from the head
 */
Node* ll_at(LinkedList *ll, size_t index) {
    if (index >= ll->size) {
        printf("Error: Index out of bounds.\n");
        return NULL;
    }

    Node *current = ll->head;

    // Advance current to the index needed
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

/*
 * Inserts a new node at the very beginning of the list
 */
int ll_insert_head(LinkedList *ll, double value) {
    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        printf("Error: Node allocation failed.\n");
        return -1;
    }

    new_node->data = value;
    new_node->next = ll->head;  // Points new node to current head
    ll->head = new_node;        // Update had to be the new node

    // If list was empty, this node is also the tail
    if (ll->size == 0) {
        ll->tail = new_node;
    }

    ll->size++;
    return 0;
}

/*
 * Inserts a new node at the very end of the list
 * This is O(1) thanks to the 'tail' pointer
 */
int ll_insert_tail(LinkedList *ll, double value) {
    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        printf("Error: Node allocation failed.\n");
        return -1;
    }

    new_node->data = value;
    new_node->next = NULL;

    if (ll->head == NULL) {
        // Case: Empty list
        ll->head = new_node;
        ll->tail = new_node;
    }
    else {
        // Case: Attach to existing tail
        ll->tail->next = new_node;
        ll->tail = new_node;
    }

    ll->size++;
    return 0;
}

/*
 * Inserts a value at a specific index
 */
int ll_insert_at(LinkedList *ll, double value, size_t index) {
    if (index == ll->size) {
        return ll_insert_tail(ll, value);
    }

    if (index == 0) {
        return ll_insert_head(ll, value);
    }

    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        printf("Error: Node allocation failed.\n");
        return -1;
    }

    // Find the node just before the target index
    Node *previous = ll_at(ll, index - 1);
    new_node->data = value;
    new_node->next = previous->next;
    previous->next = new_node;
    ll->size++;

    return 0;
}

/*
 * Deletes the first node in the list
 */
int ll_remove_head(LinkedList *ll) {
    if (!ll->head) {
        printf("Error: Head node is NULL.\n");
        return -1;
    }

    Node *buffer = ll->head;
    ll->head = buffer->next;    // Move head pointer to the second node
    free(buffer);
    ll->size--;

    // If the list is now empty, reset tail to NULL
    if (ll->size == 0) {
        ll->tail = NULL;
    }

    return 0;
}

/*
 * Deletes the last node in the list
 * This is O(n) because we must find the node BEFORE the tail
 */
int ll_remove_tail(LinkedList *ll) {
    if (ll->size == 0) {
        printf("Error: Linked list is empty.\n");
        return -1;
    }

    if (ll->size == 1) {
        return ll_remove_head(ll);
    }

    // Find the (n - 1)th node
    Node *second_to_last = ll_at(ll, ll->size - 2);
    Node *node_to_delete = second_to_last->next;

    second_to_last->next = NULL;
    ll->tail = second_to_last;  // Update tail pointer

    free(node_to_delete);
    ll->size--;
    return 0;
}

/*
 * Deletes a node at a specific position
 */
int ll_remove_at(LinkedList *ll,size_t index) {
    if (!ll->head) {
        printf("Error: Linked list is empty.\n");
        return -1;
    }

    if (index == ll->size - 1) {
        return ll_remove_tail(ll);
    }

    if (index == 0) {
        return ll_remove_head(ll);
    }

    // Get the node before the one we want to delete
    Node *current = ll_at(ll, index - 1);

    if (!current || !current->next) {
        printf("Error: Node not found at given index.\n");
        return -1;
    }

    Node *node_to_delete = current->next;
    current->next = node_to_delete->next;   // Bypass the node
    free(node_to_delete);
    ll->size--;

    return 0;
}

/*
 * Print all elements currently in the list
 */
void ll_print(LinkedList *ll) {
    if (!ll || !ll->head) {
        return;
    }

    Node *current = ll->head;
    printf("List (size %zu): ", ll->size);

    while (current != NULL) {
        printf("[%.2f] -> ", current->data);
        current = current->next;
    }

    printf("NULL\n");
}