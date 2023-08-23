#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "getWord.h"


int main (int argc, char **argv) {

    // The number of files to read is argc -1 

    int num_files_to_read = argc-1;

    if (num_files_to_read < 1){
        fprintf(stderr, "Usage: %s <file1> <file2> ... \n",argv[0] );
        exit(1);
    }

    for (int i = 0; i < num_files_to_read; i++){

        FILE *fp = NULL;
        fp = fopen(argv[i+1], "r");//open database

        if (fp == NULL) {
            fprintf(stderr, "ERROR: Cannot open  a database file\n");
        
            exit(1);
        }

        char *word;
        
        while ((word = getNextWord(fp) )!= NULL){
            
            printf("Word: %s \n", word);
            free(word);
        }

        
        fclose(fp);
    }

}