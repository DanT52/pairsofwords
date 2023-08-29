#include <stdio.h>
#include "paircounting.h"

void error_exit(char* error, hash_table * table){
    fprintf(stderr, "%s", error);
    if(table)hash_free(table, free);
    exit(1);
}

int main (int argc, char **argv) {
    int num_files_to_read = argc-2;

    int top_pairs_to_print;
    char dash;
    // The number of files to read is argc -1 
    if ((argc < 2)|| (sscanf(argv[1], "%c%d",&dash, &top_pairs_to_print) != 2) || (dash != '-') || (top_pairs_to_print <1)){
        fprintf(stderr, "Usage: %s <-count> <file1> <file2> ... \n",argv[0] );
        exit(1);
    }

    struct hash_table *table = hash_init(10, .7);
    if(!table)error_exit("ERROR: Table initialization Failed.\n", NULL);

    int result = read_files(table, num_files_to_read, argv);
    if (result == 1)error_exit("ERROR: Failed to open file.\n", table);
    if(result == 2)error_exit("ERROR: Hash Memory allocation failed.\n", table);
  
    if(top_pairs_to_print > table->count)error_exit("ERROR: Not enough Word pairs Read to Print..\n.", table);
        
    struct word_pair_count **wordpair_array = wordpairs_array(table);
    if(!wordpair_array)error_exit("ERROR: Word Pair Array allocation Failed.\n", table);

    qsort(wordpair_array, table->count, sizeof(word_pair_count*), compare_wordpairs_counts);

    for (int i = 0; i < top_pairs_to_print; i++){
        printf("%10d %s\n", wordpair_array[i]->count, wordpair_array[i]->wordpair);
    }
    free_wordpairs_array(wordpair_array, table->count);

    hash_free(table, free);

}