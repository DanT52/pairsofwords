#include <stdio.h>
#include "paircounting.h"

/*WHEN HASHTABLE RESIZES*/
/*The hashtable resizes when the max load exceeds .7*/
/*the load of hashtable is the (num of elements added)/(num of buckets)*/
/*After running a few tests I found .7 to be an optimal value for speed*/
/*higher max load values will result in less buckets but slower fetch speeds*/
/*lower max loads will be either simmilar speed or faster but will take up more buckets*/
/*see README for more details*/


//function for printing errors/freeing table and exit.
void error_exit(char* error, hash_table * table){
    fprintf(stderr, "%s", error);
    if(table)hash_free(table, free);
    exit(1);
}

int main (int argc, char **argv) {
    
    int num_files_to_read = argc-2;
    int top_pairs_to_print; 
    char dash;//place holder for dash.

    // The number of files to read is argc -2
    //ensure that the args were in the correct format.
    if ((argc < 2)|| (sscanf(argv[1], "%c%d",&dash, &top_pairs_to_print) != 2) || (dash != '-') || (top_pairs_to_print <1)){
        fprintf(stderr, "Usage: %s <-count> <file1> <file2> ... \n",argv[0] );
        exit(1);
    }

    //initialize hash_table
    //a max load of .7 is used (num items inserted/num of buckets) <= .7
    struct hash_table *table = hash_init(5, .7);
    if(!table)error_exit("ERROR: Table initialization Failed.\n", NULL);

    //read the wordpairs into hashtable
    int result = read_files(table, num_files_to_read, argv);
    if (result >= 2){
        fprintf(stderr, "ERROR: Can't read file '%s'\n", argv[result]);
        error_exit("", table);
    }
    if(result == 1)error_exit("ERROR: Hash Memory allocation failed.\n", table);
  
    //ensure enough wordpairs were read in.
    if(top_pairs_to_print > table->count)error_exit("ERROR: Not enough Word pairs Read to Print..\n.", table);
        
    //initialize word_pair_count array which will be used for sorting word pair occurances.
    struct word_pair_count **wordpair_array = wordpairs_array(table);
    if(!wordpair_array)error_exit("ERROR: Word Pair Array allocation Failed.\n", table);

    //sort the word_pair_count array
    qsort(wordpair_array, table->count, sizeof(word_pair_count*), compare_wordpairs_counts);

    //print the top_pairs_to_print word pairs.
    for (int i = 0; i < top_pairs_to_print; i++){
        printf("%10d %s\n", wordpair_array[i]->count, wordpair_array[i]->wordpair);
    }

    //free memory.
    free_wordpairs_array(wordpair_array, table->count);
    hash_free(table, free);

    return 0;
}