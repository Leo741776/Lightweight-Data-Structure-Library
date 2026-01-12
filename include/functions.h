#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Definitions for each data structure
typedef struct Stack Stack;
typedef struct Queue Queue;
typedef struct LinkedList LinkedList;
typedef struct HashTable HashTable;
typedef struct Heap Heap;

// Stack operations
Stack *s_create();
int s_free(Stack *s);
int s_push(Stack *s, double value);
double s_pop(Stack *s);
double s_peek(Stack *s);
int s_print(Stack *s);

// Queue operations
Queue *q_create();
int q_free(Queue *q);
int q_enqueue(Queue *q, double value);
double q_dequeue(Queue *q);
double q_peek(Queue *q);
int q_print(Queue *q);

// Linked list operations
LinkedList *ll_create();
int ll_free(LinkedList *ll);
int ll_insert_head(LinkedList *ll, double value);
int ll_insert_tail(LinkedList *ll, double value);
int ll_insert_at(LinkedList *ll, double value, size_t index);
int ll_remove_head(LinkedList *ll);
int ll_remove_tail(LinkedList *ll);
int ll_remove_at(LinkedList *ll,size_t index);
void ll_print(LinkedList *ll);

// Hash table operations
HashTable *ht_create();
int ht_free(HashTable *ht);
void ht_insert(HashTable *ht, char *key, double value);
int ht_delete(HashTable *ht, char *key);
double ht_search(HashTable *ht, char *key);
void ht_print(HashTable *ht);

// Heap operations
Heap *h_create();
int h_free(Heap *h);
void h_insert(Heap *h, double value);
double h_peek(Heap *h);
double h_pop_max(Heap *h);

#endif
