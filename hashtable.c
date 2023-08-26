#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "crc64.h"

hash_table* hash_init(int size, double max_load){
  //allocate for the hashtable
  hash_table* hash_entry = (hash_table *)malloc(sizeof(hash_table));
  if (!hash_entry) return NULL; // memory allloc fail

  //allocate for array of pointers to hash_nodes
  hash_entry->buckets = (hash_node **)malloc(size * sizeof(hash_node *));
  if (!hash_entry->buckets) {
      free(hash_entry);
      return NULL; // malloc failed
  }

  //set values where buckets to NULL
  for (int i = 0; i < size; i++) hash_entry->buckets[i] = NULL;

  hash_entry->size = size;
  hash_entry->count = 0;
  hash_entry->load = 0.0;
  hash_entry->max_load = max_load;

  return hash_entry;
}

int hash_insert(hash_table *table, char* key, void *value){

  struct hash_node *new_node = (hash_node *)malloc(sizeof(hash_node));
  new_node->key=key;
  new_node->value=value;
  new_node->next=NULL;

  if (!new_node) return 1;

  int bucket_num = crc64(key) % table->size;

  //the bucket is empty
  if (!table->buckets[bucket_num]){
    table->buckets[bucket_num] = new_node;
    
  } else { //find last node in chain
    struct hash_node *current = table->buckets[bucket_num];
    while(current){     
      current = current->next;
    }
    current = new_node;
    
  }

  //update stats
  table->count+=1;
  table->load = table->count/table->size;

  //resize if needed
  if (table->load >= table->max_load){
    resize_table(table);
  }
  
  return 0; //sucess insert
}

void resize_table(hash_table *table){
  //save old size
  int old_size = table->size; 

  //multiply table size by 3
  table->size = old_size *3;
  //count set back to 0 (all nodes will be reinserted)
  table->count = 0;

  struct hash_node **old_buckets = table->buckets;
  //allocate mem for new buckets
  struct hash_node **new_buckets = (hash_node **)malloc(table->size * sizeof(hash_node *));
  //set buckets to NULL
  for (int i = 0; i < table->size; i++) {
        new_buckets[i] = NULL;
    }
  //add new buckets to table
  table->buckets = new_buckets;
  
  //reinsert all the hash_node elements
  for (int i = 0; i < old_size; i++){
    struct hash_node *current = table->buckets[i];
    while(current){
      struct hash_node *temp = current;
      hash_insert(table, current->key, current->value);
      current = current->next;
      free(temp); //free the old hash_nodes
    }
  }
  free(old_buckets); //free the old bucket pointer array
  
}