# pairsofwords
Systems Programming - CS 360 Assignment 1



# hash table part
insert, search, free.

#Ideas

go through each file and hash the pairs of words,
check if already in hashtable

if yes then increment count inside the word_pair struct

if not in table, hash and add to table with count of 0 in word pair struct.

after all words have been read in, go through the hashtable and take out the word_pair structs to make a big array.

Use qsort on that array with a custom compare function.

print the top.






i