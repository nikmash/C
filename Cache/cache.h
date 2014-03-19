/*
* Search.h
*/
#ifndef SEARCH_H
#define SEARCH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


struct llnode_{
	char *filename;
	struct llnode_ *next;
};
typedef struct llnode_ llnode;

struct linkedlist{
	llnode *head;
	int size;
};
typedef struct linkedlist* linkedListPtr;

struct words{
	char* string;
	int counter;
	int memory;
	linkedListPtr filelist;
};

typedef struct words word;



#endif
