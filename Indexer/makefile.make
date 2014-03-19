COMPILER = gcc

all: indexer

indexer: indexer.o sorted-list.o tokenizer.o
	$(COMPILER) -o indexer indexer.o sorted-list.o tokenizer.o

tokenizer.o: tokenizer.c
	$(COMPILER) -c tokenizer.c

sorted-list.o: sorted-list.c
	$(COMPILER) -c sorted-list.c

indexer.o: indexer.o
	$(COMPILER) -c indexer.c

clean:
	rm -f indexer *.o
