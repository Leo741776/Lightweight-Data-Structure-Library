#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 100            // The starting number of buckets within the hash table

// Each entry acts as a node in a linked list (separate chaining is used)
typedef struct Entry {
    char *key;          // Unique identifier
    double value;       // Data stored
    struct Entry *next; // Pointer to the next entry in the same bucket
} Entry;

// The main hash table structure
typedef struct HashTable {
    Entry **buckets;        // Array of pointers to Entr (the "bucket" heads)
    size_t count;           // Total number of key-value pairs in the table
    size_t capacity_table;  // Number of available buckets
} HashTable;

/*
 * Creates and initializes a new Hash Table
 * Uses calloc to make sure all bucket pointers start as NULL
 */
HashTable *ht_create() {
    HashTable *ht = malloc(sizeof(HashTable));

    if (!ht) {
        printf("Error: Hash table allocation failed.\n");
        return NULL;
    }

    // calloc initializes the memory to zero, making all buckets NULL by default
    ht->buckets = calloc(DEFAULT_SIZE, sizeof(Entry *));

    if (!ht->buckets) {
        free(ht);
        return NULL;
    }

    ht->count = 0;
    ht->capacity_table = DEFAULT_SIZE;

    return ht;
}

/*
 * Deeply frees the hash table, including all bucket chains and keys
 */
int ht_free(HashTable *ht) {
    if (!ht) {
        printf("Error: Hash table is NULL.\n");
        return -1;
    }

    if (ht->buckets != NULL) {
        for (size_t i = 0; i < ht->capacity_table; i++) {
            Entry *current_entry = ht->buckets[i];
            Entry *next_entry;

            // Traverse and free the linked list in each bucket
            while (current_entry != NULL) {
                next_entry = current_entry->next;
                free(current_entry->key);   // Key was allocated via strdup
                free(current_entry);
                current_entry = next_entry;
            }
        }
        free(ht->buckets);
    }

    free(ht);

    return 0;
}

/*
 * Uses a djb2 hash function, good for string hashing
 * Transforms a string into a large unsigned integer
 */
unsigned int ht_hash(char *key) {
    if (!key) {
        printf("Error: Key is NULL.\n");
        return -1;
    }

    unsigned int hash = 5381;   // Prime starting constant

    for (size_t i = 0; key[i] != '\0'; i++) {
        // hash * 33 + char
        hash = ((hash << 5) + hash) + key[i];
    }

    return hash;
}

/*
 * Resizes the table when the load factor (count/capacity) exceeds the threshold (0.7)
 * Re-hashes all existing entries into a new, larger bucket array
 */
int ht_rehash(HashTable *ht) {
    if (!ht) {
        return -1;
    }

    size_t old_capacity = ht->capacity_table;
    size_t new_capacity = ht->capacity_table * 2;
    Entry **new_buckets = calloc(new_capacity, sizeof(Entry *));

    if (!new_buckets) {
        return -1;
    }

    // Move every entry from the old buckets to the new ones
    for (size_t i = 0; i < old_capacity; i++) {
        Entry *current_entry = ht->buckets[i];
        while (current_entry != NULL) {
            Entry *next_entry = current_entry->next;

            // Re-calculate the index based on the NEW capacity
            unsigned int new_index = ht_hash(current_entry->key) % new_capacity;

            // Insert at the beginning of the new bucket's list
            current_entry->next = new_buckets[new_index];
            new_buckets[new_index] = current_entry;

            current_entry = next_entry;
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity_table = new_capacity;

    return 0;
}

/*
 * Inserts or updates a key-value pair
 * Maintains a maximum load factor of 0.7 to keep performance steady
 */
void ht_insert(HashTable *ht, char *key, double value) {
    if (!ht || !key) {
        printf("Error: Hash table is NULL.\n");
        return;
    }

    // Check load factor: if > 70%, double the table size
    if ((double) ht->count / ht->capacity_table > 0.7) {
        ht_rehash(ht);
    }

    unsigned int hash_value = ht_hash(key);
    size_t index = (size_t) hash_value % ht->capacity_table;
    Entry *current_entry = ht->buckets[index];

    // Check if key already exists (update case)
    while (current_entry != NULL) {
        if (strcmp(current_entry->key, key) == 0) {
            current_entry->value = value;
            return;
        }
        current_entry = current_entry->next;
    }

    // Key doesn't exist, create a new entry (insertion case)
    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry) return;

    new_entry->key = strdup(key);   // Duplicate the string to manage its own memory
    if (!new_entry->key) {
        free(new_entry);
        printf("Error: Key allocation failed.\n");
        return;
    }

    new_entry->value = value;
    // Push to the front of the linked list (head insertion)
    new_entry->next = ht->buckets[index];
    ht->buckets[index] = new_entry;
    ht->count++;
}

/*
 * Removes a key and its value from the table
 */
int ht_delete(HashTable *ht, char *key) {
    if (!ht || !key) {
        printf("Error: Hash table and/or key is NULL.\n");
        return -1;
    }

    size_t index = ht_hash(key) % ht->capacity_table;

    Entry *current_entry = ht->buckets[index];
    Entry *previous_entry = NULL;

    while (current_entry != NULL) {
        if (strcmp(current_entry->key, key) == 0) {

            // Unlink the node from the chain
            if (previous_entry == NULL) {
                ht->buckets[index] = current_entry->next; // Node was the head
            } else {
                previous_entry->next = current_entry->next; // Node was in the middle/end
            }

            free(current_entry->key);
            free(current_entry);

            ht->count--;
            return 0;
        }
        previous_entry = current_entry;
        current_entry = current_entry->next;
    }

    return -1;  // Key not found
}

/*
 * Searches for a key and returns a pointer to the value if found
 */
double ht_search(HashTable *ht, char *key) {
    if (!ht || !key) {
        printf("Error: Hash table and/or key is NULL.\n");
        return NAN;
    }

    size_t index = ht_hash(key) % ht->capacity_table;

    Entry *current_entry = ht->buckets[index];

    while (current_entry != NULL) {
        if (strcmp(current_entry->key, key) == 0) {
            return current_entry->value;
        }
        current_entry = current_entry->next;
    }

    return NAN;
}

/*
 * Prints a visualization of the hash table and its bucket chains
 */
void ht_print(HashTable *ht) {
    for (size_t i = 0; i < ht->capacity_table; i++) {
        printf("Bucket %zu: ", i);
        Entry *current_entry = ht->buckets[i];
        while (current_entry) {
            printf("[%s: %.2f] -> ", current_entry->key, current_entry->value);
            current_entry = current_entry->next;
        }
        printf("NULL\n");
    }
}
