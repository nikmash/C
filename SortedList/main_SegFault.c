/*
 * main.c
 */

#include    <stdio.h>
#include    <stdlib.h>
#include	<string.h>
#include	"sorted-list.h"


int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}


int main(){
	printf("WHEEE");
	int x;
	int i;
	int numbers[] = {12, 6, 3, 2, 5, 23, 19, 5, 12}; //array has to be of type int....no need for quotes
	x = sizeof(numbers)/sizeof(int); //this is how u find out the size of array
	int (*PF) (void*, void*);
	PF = compareInts;
	SortedListPtr newList = SLCreate(PF);



	for(i = 0; i < x; i++){
        int *num = (int *) malloc(sizeof(int)); //you need to make space in memory for one int and set it to the current int in the arrray
        *num = numbers[i];
		SLInsert(newList, num); //passing the address of that int in the memory to the program
	}
    
	SortedListIteratorPtr SPtr = SLCreateIterator(newList);
    
    //goes through the list and prints the items out
	listNode* curr = (listNode *) SLNextItem(SPtr);
	while (curr != NULL) {
        int *n = (int *)curr->data;
		printf("%d -> ", *n);
		curr = (listNode *) SLNextItem(SPtr);
    }
    
	SLDestroyIterator(SPtr);
	SLDestroy(newList);
}
