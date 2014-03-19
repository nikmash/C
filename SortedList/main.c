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
/*
Our main function allows the user to test our sorted list 
with three different types of data. Enter "int" to test 
our array of Integers. Enter "string" to test our array 
of words. Enter "double" to test our array of doubles.
*/

 int main(int argc, char **argv){
 	if(argc == 1){
 		printf("Invalid number of arguments.");
 	}
 	if (strcmp(argv[1], "int") == 0){

	 	int numbers[] = {328, 3832, 38, 6739, 289, 5, 21873, 8210, 39, 490, 382, 382, 56};


	 	printf("got here \n");
	 	
	 	int(*CF)(void*, void*);
	 	CF = compareInts;

	 	SortedListPtr list = SLCreate(CF);
	 	
	 	int size = sizeof(numbers)/sizeof(int);
	 	int i;

	 	for(i = 0; i < size; i++){
	 	 int *num = (int *) malloc(sizeof(int));
	 	 *num = numbers[i];
	 	 SLInsert(list, num);
	 	 num = NULL;

	 	}

	 	int *num2 = (int *) malloc(sizeof(int));
	 	*num2 = 289;

	 	SLRemove(list, num2);
	 	
	 	SortedListIteratorPtr iter = SLCreateIterator(list);

	 	printf("Sorted List \n"); /* This shows that the Iterator works and prints out all the items in the sorted list using the iterator*/
	 	int j;
	 	for(j = 0; j < list->size; j++){
	 		listNode *node = (listNode *) malloc(sizeof(listNode));
	 		node = SLNextItem(iter);
	 		if(node == NULL){
	 			break;
	 		}

	 		printf("%d \n", *((int *)(node->data)));
	 		nodeDestroy(node);
	 	}
	 	SLDestroy(list);
	 	SLDestroyIterator(iter);



 	}

 	if(strcmp(argv[1], "string") == 0){
	 	char *string[] = {"hello", "this", "class", "is", "great"};

	 	int(*SF)(void*, void*);
	 	SF = compareStrings;

	 	SortedListPtr list2 = SLCreate(SF);

	 	int x;

	 	for(x= 0; x < 5; x++){
	 	 
	 	 SLInsert(list2, string[x]);

	 	}

	 	SLRemove(list2, string[1]);
	 	
	 	SortedListIteratorPtr iter2 = SLCreateIterator(list2);

	 	printf("Sorted List \n"); /* This shows that the Iterator works and prints out all the items in the sorted list using the iterator*/
	 	int k;
	 	for(k = 0; k < list2->size; k++){
	 		listNode *node = (listNode *) malloc(sizeof(listNode));
	 		node = SLNextItem(iter2);
	 		if(node == NULL){
	 			break;
	 		}

	 		printf("%s \n", (char *)(node->data));
	 	}

	 	printf("%d this is the number of things left in the list \n", list2->size);

	 	}

	 if(strcmp(argv[1], "double") == 0){
	 	
	 	double numbers[] = {3.0, 7.3, 7.0, 6.0, 289.9, 333.3, 2100.6};


	 	printf("got here \n");
	 	
	 	int(*GF)(void*, void*);
	 	GF = compareDoubles;

	 	SortedListPtr list3 = SLCreate(GF);
	 	
	 	int size = sizeof(numbers)/sizeof(double);
	 	int i;

	 	for(i = 0; i < size; i++){
	 	 double *num2 = (double *) malloc(sizeof(double));
	 	 *num2 = numbers[i];
	 	 SLInsert(list3, num2);
	 	 num2 = NULL;

	 	}

	 	double *num2 = (double *) malloc(sizeof(double));
	 	*num2 = 6.0;

	 	SLRemove(list3, num2);
	 	
	 	SortedListIteratorPtr iter = SLCreateIterator(list3);

	 	printf("Sorted List \n"); /* This shows that the Iterator works and prints out all the items in the sorted list using the iterator*/
	 	int j;
	 	for(j = 0; j < list3->size; j++){
	 		listNode *node = (listNode *) malloc(sizeof(listNode));
	 		node = SLNextItem(iter);
	 		if(node == NULL){
	 			break;
	 		}

	 		printf("%f \n", *((double *)(node->data)));
			
	 	}
	 	SLDestroy(list3);
	 	SLDestroyIterator(iter);


	 }


}
 	

