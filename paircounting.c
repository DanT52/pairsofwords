#include <stdio.h>
#include "paircounting.h"

char* combine_words(char* word1, char* word2){
    //reqired length is word1+word2 +space + \0
    unsigned int combined_size = strlen(word1) + strlen(word2) + sizeof(char)*2;
    //allocate space for wordpair return NULL if fail
    char *wordpair = (char*)malloc(combined_size);
    if (!wordpair)return NULL; 
    sprintf(wordpair, "%s %s", word1, word2);

    return wordpair;
}

int read_files(hash_table *table, int num_of_files, char **file_names){
    
    char *word1;
    char *word2;
    char *wordpair; //pointer for the wordpair.

    for (int i = 0; i < num_of_files; i++){ //run for each file given

        FILE *fp = NULL;
        fp = fopen(file_names[i+2], "r");//open file
        if (!fp)return i+2; //failed to open

        word1 =getNextWord(fp); //get the first word
        if (!word1){ //empty file
            fprintf(stderr, "Notice: File %s was empty.\n", file_names[i+2]);
            fclose(fp);
            continue;
        }
        //keep getting a word till end of file
        while ((word2 = getNextWord(fp) )!= NULL){

            wordpair = combine_words(word1, word2); //generate the word pair string.
            if(!wordpair){ //failed to allocate
                free(word1);
                free(word2);
                fclose(fp);
                return 1;
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
                    return 1;
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
    //allocate space for wordpairs array
    struct word_pair_count **wordpairs = (word_pair_count**)malloc(sizeof(word_pair_count*)* table->count);
    if (!wordpairs)return NULL;

    int insert_count = 0; //count for #th hashnode being inserted

    for (int i = 0; i < table->size; i++) { //run for each bucket of hash_node array
        hash_node *current = table->buckets[i];
        while(current){ //run for each hash_node in the bucket.
            
            //allocate word_pair_count node
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
    //read in void pointers as word_pair_count structs
    word_pair_count *pair1 = *(word_pair_count **)a;
    word_pair_count *pair2 = *(word_pair_count **)b;
    //sort descending order.
    return pair2->count - pair1->count;
}


void free_wordpairs_array(word_pair_count ** wordpair_array, int size){
    for (int i = 0; i < size; i++){ //free nodes
        free(wordpair_array[i]);
    }
    free(wordpair_array);//free array of pointies
}



void print_wordpairs_array(word_pair_count ** wordpair_array, int size){
    printf("---- WORD PAIR ARRAY ----\n");
    for (int i = 0; i < size; i++){
        printf("[%s -> %d]\n", wordpair_array[i]->wordpair, wordpair_array[i]->count);
    }
    printf("---------------------\n");
    
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



