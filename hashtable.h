#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "crc64.h"

//hash node struct
//keys are strings and values are void pointers.
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

//initializes hashtable. Size is the number of initial buckets being added.
//max_load is a double representing the max_load allowed before the hashtable size is trippled.
//returns NULL if malloc failed
hash_table* hash_init(int size, double max_load);

//insert a new key value pair into hashtable. 
//key is hashed using crc64
//key and value are directly inserted into the table
//count and load are updated
//resize_table is run if the max load is exceeded
//returns 1 if insert failed.
int hash_insert(hash_table *table, char* key, void *value);

//Increases bucket amount by a factor of 3.
//new bucket array is created and all the elements in the current are re hashed and inserted into new array.
//new bucket array is added to hash_table and old one is freed.
void resize_table(hash_table *table); // 3x table


//runs hash function on key, looks in appropriate bucket for a string matching key,
//if found returns the void pointer "value",
// returns NULL if not found
void* hash_find(hash_table *table, char *key);

//takes in the table and a pointer to the function needed to free the value.
//frees each hash_node in the bucket array, including the key and value, 
//frees the bucket array and hash_table struct itself
void hash_free(hash_table *table, void (*free_value)(void* val));


#endif
