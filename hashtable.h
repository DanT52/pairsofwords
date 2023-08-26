#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hash_node {
    char* key;
    void* value;
    struct hash_node *next;
} hash_node;

typedef struct hash_table {
    int size; //(hashmap size)
    int count; //(number of items stored in hashmap)
    double load; // (count/ size)
    double max_load; //max load allowed before resize
    struct hash_node **buckets;   //array of pointies
} hash_table;

hash_table* hash_init(int size, double max_load);

int hash_insert(hash_table *table, char* key, void *value);

void* hash_find(hash_table *table, void *key);

void resize_table(hash_table *table); // 3x table

void hash_free(hash_table *table);

#endif
