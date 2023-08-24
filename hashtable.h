#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hash_node {
    void *key;
    void *value;
    struct hash_node *next;
} hash_node;

typedef struct hash_table {
    int size; //(hashmap size)
    int count; //(number of items stored in hashmap)
    double load; // (count/ size)
    double max_load; //max load allowed before resize
    struct hash_node **buckets;   //array of pointies
} hash_table;

hash_table* hashinit(int initial_size);

void hash_insert(hash_table_t *table, void *key, void *value);

void* hash_find(hash_table_t *table, void *key);

void hash_delete(hash_table_t *table, void *key);

static void resize_table(hash_table_t *table);

void hash_free(hash_table_t *table);

#endif
