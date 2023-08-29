#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdlib.h>
#include "getWord.h"
#include "hashtable.h"

//wordpair struct, this is used for creating an array of the wordpairs and their counts
//the array of this struct is then sorted with qsort.
typedef struct word_pair_count {
    char* wordpair;
    int count;
}word_pair_count;

//takes in the hashtable full of wordpairs and counts.
//returns an unordered array of word_pair_count structs created from the hashnodes.
//returns NULL if error.
word_pair_count ** wordpairs_array(hash_table *table);

//frees just the space for the array of word_pair_count structs in the array,
//since the pointers to values inside are freed with the hashtable.
void free_wordpairs_array(word_pair_count ** wordpair_array, int size);

//reads wordpairs from a list of files comming in from an array of strings.
//reads the strings from index 2 as file names and attemps to open and read the wordpairs.
//the wordpairs are stored in the hashtable with the key being the wordpair and the count 
//the number of times the pair occured.
//returned 1, failed to open, returned 2, malloc fail, returned 0 sucess.
int read_files(hash_table *table, int num_of_files, char **file_names);

//combines two strings into a new allocated string with a space in between.
//returns the wordpair string NULL if allocation failed.
char* combine_words(char* word1, char* word2);

//compares word_pair_count strings by interger value in decending order
//returns < 0 when a>b,  0 a == b, >0 b>a
int compare_wordpairs_counts(const void *a, const void *b );


//printing functions used for debugging, cycle through the hashtable or struct array.
//print all the values stored.
void print_wordpairs_array(word_pair_count ** wordpair_array, int size);
void print_hash_table(hash_table *table);


#endif