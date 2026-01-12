#include <stdio.h>

#include "functions.h"

int main() {
    printf("----------Stack outputs----------\n");
    Stack *s = s_create();

    s_push(s, 465.23);
    s_push(s, 38136.427);
    s_push(s, -55.2);
    s_push(s, -275.565);
    s_push(s, 37.553);

    printf("%f\n", s_pop(s));

    printf("%f\n", s_peek(s));

    s_print(s);

    s_free(s);
    s = NULL;

    printf("----------Queue outputs----------\n");
    Queue *q = q_create();

    q_enqueue(q, 42.53);
    q_enqueue(q, 345.1);
    q_enqueue(q, 22.2);
    q_enqueue(q, 536.727);
    q_enqueue(q, 904.330);

    printf("%.2f\n", q_dequeue(q));

    printf("%.2f\n", q_peek(q));

    q_print(q);

    q_free(q);
    q = NULL;

    printf("----------Linked list outputs----------\n");
    LinkedList *ll = ll_create();

    ll_insert_head(ll, 254.12);
    ll_insert_tail(ll, 4077.076);
    ll_insert_head(ll, 8674.949);
    ll_insert_tail(ll, 5515.686);
    ll_insert_at(ll, 5393.824, 2);

    ll_remove_at(ll, 3);
    ll_remove_head(ll);
    ll_remove_tail(ll);

    ll_print(ll);

    ll_free(ll);
    ll = NULL;

    printf("----------Hash table outputs----------\n");
    HashTable *ht = ht_create();

    ht_insert(ht, "Adam", 140.769);
    ht_insert(ht, "Stacey", 963.899);
    ht_insert(ht, "Joseph", 29.658);
    ht_insert(ht, "Bill", 844.740);
    ht_insert(ht, "Emily", 746.524);

    ht_delete(ht, "Adam");

    char *name = "Emily";
    printf("%.2f\n", ht_search(ht, name));

    ht_free(ht);
    ht = NULL;

    printf("----------Max heap outputs----------\n");
    Heap *h = h_create();

    h_insert(h, 545.033);
    h_insert(h, 749.643);
    h_insert(h, 469.843);
    h_insert(h, 784.714);
    h_insert(h, 521.641);

    printf("%.2f\n", h_pop_max(h));

    printf("%.2f\n", h_peek(h));

    h_free(h);
    h = NULL;
    return 0;
}