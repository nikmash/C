COMPILER = gcc
LDFLAGS = -W

all: Cache

Cache: cache.o tokenizer.o
	$(COMPILER) -o cache cache.o tokenizer.o

tokenizer.o: tokenizer.c
	$(COMPILER) -c tokenizer.c

cache.o: cache.c
	$(COMPILER) -c cache.c

clean:
	rm -f Cache *.o
