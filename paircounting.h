#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdlib.h>
#include "getWord.h"
#include "hashtable.h"


typedef struct word_pair_count {
    char* wordpair;
    int count;
}word_pair_count;

word_pair_count ** wordpairs_array(hash_table *table);
int read_files(hash_table *table, int num_of_files, char **file_names);
char* combine_words(char* word1, char* word2);
word_pair_count ** wordpairs_array(hash_table *table);
int compare_wordpairs_counts(const void *a, const void *b );

void free_wordpairs_array(word_pair_count ** wordpair_array, int size);

void print_wordpairs_array(word_pair_count ** wordpair_array, int size);
void print_hash_table(hash_table *table);


#endif