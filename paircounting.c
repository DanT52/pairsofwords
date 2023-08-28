#include <stdio.h>
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

char* combine_words(char* word1, char* word2){
    //reqired length is word1+word2 +space + \0

    unsigned int combined_size = strlen(word1) + strlen(word2) + sizeof(char)*2;
    char *wordpair = (char*)malloc(combined_size);
    if (!wordpair)return NULL;
    sprintf(wordpair, "%s %s", word1, word2);
    return wordpair;

}


int read_files(hash_table *table, int num_of_files, char **file_names){

    char *word1;
    char *word2;
    char *wordpair;

    for (int i = 0; i < num_of_files; i++){ //run for each file given
        

        FILE *fp = NULL;
        fp = fopen(file_names[i+2], "r");//open file
        if (!fp)return 1; //failed to open

        word1 =getNextWord(fp); //get the first word
        if (!word1){ //empty file
            fprintf(stderr, "Notice: File %s was empty.\n", file_names[i+2]);
            fclose(fp);
            continue;
        }
        //keep getting a word till end of file
        while ((word2 = getNextWord(fp) )!= NULL){

            wordpair = combine_words(word1, word2);
            
            if(!wordpair){ //failed to allocate
                free(word1);
                free(word2);
                fclose(fp);
                return 2;
            }
            //see if wordpair already in table
                       
            void* ptr = hash_find(table, wordpair);
            
            if(!ptr){ //not in table add with 1 as value
                int* num_ptr = (int*) malloc(sizeof(int));
                *num_ptr = 1;
                if (hash_insert(table, wordpair, num_ptr) != 0){ 
                    //failed to allocate
                    free(word1);
                    free(word2);
                    fclose(fp);
                    return 2;
                }
            }else{ // wordpair already in table increment value
                (*(int*)ptr)++;
                free(wordpair); //word pair is freed here since its not added to table
            }
            free(word1);
            word1 = word2;
        }
        free(word1);
        
        fclose(fp);
        
    }
    return 0;

}

word_pair_count ** wordpairs_array(hash_table *table){
    struct word_pair_count **wordpairs = (word_pair_count**)malloc(sizeof(word_pair_count*)* table->count);
    if (!wordpairs)return NULL;

    int insert_count = 0;
    for (int i = 0; i < table->size; i++) {
        hash_node *current = table->buckets[i];
        while(current){
            wordpairs[insert_count] = (word_pair_count*)malloc(sizeof(word_pair_count));
            if (!wordpairs[insert_count]) return NULL;

            wordpairs[insert_count]->wordpair = current->key;
            wordpairs[insert_count]->count = *((int*)current->value);

            current = current->next;
            insert_count +=1;
        }
    }

    return wordpairs;
}

int compare_wordpairs_counts(const void *a, const void *b ){
    word_pair_count *pair1 = *(word_pair_count **)a;
    word_pair_count *pair2 = *(word_pair_count **)b;

    return pair2->count - pair1->count;
}

void free_wordpairs_array(word_pair_count ** wordpair_array, int size){
    for (int i = 0; i < size; i++){
        free(wordpair_array[i]);
    }
    free(wordpair_array);
}



void print_wordpairs_array(word_pair_count ** wordpair_array, int size){
    printf("---- WORD PAIR ARRAY ----\n");
    for (int i = 0; i < size; i++){
        printf("[%s -> %d]\n", wordpair_array[i]->wordpair, wordpair_array[i]->count);
    }
    
}

void print_hash_table(hash_table *table) {
    printf("---- HASH TABLE ----\n");
    for (int i = 0; i < table->size; i++) {
        printf("Bucket %d: ", i);
        hash_node *current = table->buckets[i];
        while (current) {
            printf("[%s -> %d] -> ", current->key, *(int*)current->value);  
            current = current->next;
        }
        printf("NULL\n");
    }
    printf("---------------------\n");
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
    if(!table){
        fprintf(stderr, "ERROR: Table initialization Failed.\n");
        exit(1);
    }
    int result = read_files(table, num_files_to_read, argv);
    if (result == 1){
        hash_free(table, free);
        fprintf(stderr, "ERROR: Failed to open file.\n");
        exit(1);
    }else if(result == 2){
        hash_free(table, free);
        fprintf(stderr, "ERROR: Hash Memory allocation failed.\n");
        exit(1);
    }
    if(top_pairs_to_print > table->count){
        fprintf(stderr, "ERROR: Not enough Word pairs Read to Print..\n.");
        hash_free(table, free);
        exit(1);
    }

    struct word_pair_count **wordpair_array = wordpairs_array(table);
    if(!wordpair_array){
        fprintf(stderr, "ERROR: Word Pair Array allocation Failed.\n");
        hash_free(table, free);
        exit(1);
    }

    qsort(wordpair_array, table->count, sizeof(word_pair_count*), compare_wordpairs_counts);

    for (int i = 0; i < top_pairs_to_print; i++){
        printf("%10d %s\n", wordpair_array[i]->count, wordpair_array[i]->wordpair);
    }
    free_wordpairs_array(wordpair_array, table->count);

    hash_free(table, free);

}