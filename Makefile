paircounting: paircounting.o getWord.o hashtable.o crc64.o
	gcc -o paircounting paircounting.o getWord.o hashtable.o crc64.o

paircounting.o: paircounting.c getWord.h hashtable.h
	gcc -c paircounting.c

getWord.o: getWord.c getWord.h
	gcc -c getWord.c

hashtable.o: hashtable.c hashtable.h crc64.h
	gcc -c hashtable.c

crc64.o: crc64.c crc64.h
	gcc -c crc64.c

debug:
	gcc -g -c getWord.c
	gcc -g -c hashtable.c
	gcc -g -c crc64.c
	gcc -g -c paircounting.c
	gcc -g -o paircounting paircounting.o getWord.o hashtable.o crc64.o

clean:
	rm paircounting paircounting.o getWord.o hashtable.o crc64.o
