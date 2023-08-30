# pairsofwords
Systems Programming - CS 360 Assignment 1

## How to Compile and Run

The Program can be compiled with:
    
    make

The output executable is ``"pairsofwords"``

Usage: **./pairsofwords -count file1 file2 ...**

Example of running the program is as follows:

    ./pairsofwords -5 example.txt example2.txt

The command above will read the files example.txt and example2.txt and output the 5 most occuring wordpairs in those files.

Removing .o files and executable:

    make clean

### Compiling without the Makefile

To complie without using the make file run:

    gcc -o pairsofwords crc64.c hashtable.c getWord.c paircounting.c pairsofwords.c


## Finding Optimal Max Load for Hash table

I tested the program with all the datasets avalible. The initial hash table size used was 5 buckets.

### HashTable Load

Load is a measurment of (Items in table)/(number of Buckets). Max Load is used as the maximum load allowed before the table resizes. The goal is to maintain a O(1) time for fetching a value from the table. 


command used to test:

    ./pairsofwords -3 datasets/dracula.txt datasets/mobydick.txt datasets/gettysburg.txt datasets/frankenstein.txt

Results:

| Max Load | Avg Time (s) | Final Table Buckets |
|----------|--------------|---------------------|
| .1       | 0.274        | 295245              |
| .7       | 0.259        | 295245              |
| 1.5      | 0.295        | 295245              |
| 10       | 0.324        | 32805               |


### Conclusion

- Max Loads between .1 - 1.5 result in the same final number of buckets since the number of buckets is tripled each time the max load is reached. 
- The fastest times occured at a .7 max load.
- Once the max load is made > 2 the final bucket count decreases but the program gets slower.

A max load of .7 will be used in this program.