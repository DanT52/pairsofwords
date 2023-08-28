#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
    while(current->next){     
      current = current->next;
    }
    current->next = new_node;
    
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
  printf("resized\n");
  //save old size
  int old_size = table->size; 

  //multiply table size by 3
  table->size = old_size *3;
  //count set back to 0 (all nodes will be reinserted)
  table->count = 0;

  struct hash_node **old_buckets = table->buckets;
  //allocate mem for new buckets
  struct hash_node **new_buckets = (hash_node **)malloc(table->size * sizeof(hash_node *));
  assert(new_buckets != NULL && "resize_table: Memory allocation for new_buckets failed.");
  //set buckets to NULL
  for (int i = 0; i < table->size; i++) {
        new_buckets[i] = NULL;
  }
  //add new buckets to table
  table->buckets = new_buckets;
  
  //reinsert all the hash_node elements
  for (int i = 0; i < old_size; i++){
    struct hash_node *current = old_buckets[i];
    while(current){
      struct hash_node *temp = current;
       int result = hash_insert(table, current->key, current->value);
      // assert to check if hash_insert returned 0
      assert(result == 0 && "resize_table: hash_insert did not return 0.");
      current = current->next;
      free(temp); //free the old hash_nodes
    }
  }
  free(old_buckets); //free the old bucket pointer array
  
}


void hash_free(hash_table *table, void (*free_value)(void* val)){

  //for each bucket in the hashtable
  for (int i = 0; i < table->size; i++) {
    
    struct hash_node *current = table->buckets[i];

    //traverse the buckets Linked List free each item.
    while (current) {
      
      struct hash_node *temp = current;
      current = current->next;
      free(temp->key);

      //if a free function for the void* value is provided use it.
      if(free_value)free_value(temp->value);
      
      free(temp);
    }
    
    
  }

  //free the buckets array  and the table itself.
  free(table->buckets);
  free(table);

}

void* hash_find(hash_table *table, char *key){

  //hash the key to know which bucket
  int bucket_num = crc64(key) % table->size;

  struct hash_node *current = table->buckets[bucket_num];

  //traverse buckets too see if any keys match.
  while (current) {
    if (strcmp(current->key, key) == 0){

      //returne value if key match
      return current->value;
    }
  }
  //NULL returned not found.
  return NULL;
}