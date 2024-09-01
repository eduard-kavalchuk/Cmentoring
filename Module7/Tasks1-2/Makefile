CC = gcc
CFLAGS = -Wall -O3 -c -std=gnu99 -Iinclude
OUTPUT = duplicates

all: bst.o set.o main.o
	$(CC) -o $(OUTPUT) bst.o set.o main.o

bst.o:
	$(CC) $(CFLAGS) bst.c

set.o:
	$(CC) $(CFLAGS) set.c
    
main.o:
	$(CC) $(CFLAGS) main.c
    
.PHONY:
	clean
    
clean:
	rm -fr duplicates *.o
