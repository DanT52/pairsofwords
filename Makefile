#compiler
CC = gcc


# object files
OBJS = crc64.o hashtable.o getWord.o paircounting.o pairsofwords.o

# main target
pairsofwords: $(OBJS)
	$(CC) -o pairsofwords $(OBJS)

# dependencies
crc64.o: crc64.c crc64.h
	$(CC) -c crc64.c

hashtable.o: hashtable.c hashtable.h crc64.h
	$(CC) -c hashtable.c

getWord.o: getWord.c getWord.h
	$(CC) -c getWord.c

paircounting.o: paircounting.c paircounting.h getWord.h hashtable.h
	$(CC) -c paircounting.c

pairsofwords.o: pairsofwords.c hashtable.h paircounting.h
	$(CC) -c pairsofwords.c


# run

run: pairsofwords -5 mobydick.txt
	./pairsofwords

	
# clean target
clean:
	rm -f $(OBJS) pairsofwords
