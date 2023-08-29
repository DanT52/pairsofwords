
CC = gcc
CFLAGS = -Wall

# object files
OBJS = crc64.o hashtable.o getWord.o paircounting.o pairsofwords.o

# main target
pairsofwords: $(OBJS)
	$(CC) $(CFLAGS) -o pairsofwords $(OBJS)

# dependencies
crc64.o: crc64.c crc64.h
	$(CC) $(CFLAGS) -c crc64.c

hashtable.o: hashtable.c hashtable.h crc64.h
	$(CC) $(CFLAGS) -c hashtable.c

getWord.o: getWord.c getWord.h
	$(CC) $(CFLAGS) -c getWord.c

paircounting.o: paircounting.c paircounting.h getWord.h hashtable.h
	$(CC) $(CFLAGS) -c paircounting.c

pairsofwords.o: pairsofwords.c hashtable.h paircounting.h
	$(CC) $(CFLAGS) -c pairsofwords.c

# clean target
clean:
	rm -f $(OBJS) pairsofwords
