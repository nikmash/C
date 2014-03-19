/*
*sorted-list.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorted-list.h"

listNode *nodeCreate(void *data, listNode *next){
	listNode *node = (listNode *) malloc(sizeof(listNode));
	if(node == NULL) return NULL;

	linkedListPtr llist = (linkedListPtr) malloc(sizeof(struct linkedlist));
	if(llist == NULL) return NULL;

	node->list = llist;
	node->list->head = NULL;
	node->list->size = 0;

	node->data = data;
	node->next = next;
	return node;
}

llnode *llnodeCreate(char *filename){
	llnode *node = (llnode *) malloc(sizeof(struct llnode_));
	node->filename = filename;
	node->freq = 1;

	return node;
}

void nodeDestroy(listNode *node){
	if(node == NULL){
		return;
	}

	free(node);
}

void llnodeDestroy(llnode *node){
	if(node == NULL)
		return;

	free(node);
}


SortedListPtr SLCreate(CompareFuncT cf){
	

	SortedListPtr Slist = (SortedListPtr) malloc(sizeof(struct SortedList));
	if(Slist == NULL) return NULL;

	Slist->head = NULL;
	Slist->comp = cf;
	Slist->size = 0;

	return Slist;
}


void SLDestroy(SortedListPtr list){
	
	SortedListPtr newlist = list;
	listNode *current = newlist->head;
	listNode *next = NULL;

	llnode *current1;
	llnode *next1;
	if (current->next == NULL){
		free(current);
	}
	else {
		
		while (current->next != NULL){
			
			next = current->next;
			
			current1 = current->list->head;
			next1 = NULL;
			
			if(current1->next == NULL){
				free(current1);
			}

			else{
				
				while(current1->next != NULL){
					next1 = current1->next;
					llnodeDestroy(current1);
					current1 = next1;
				}
			}

			nodeDestroy(current);
			current = next;
		}
		nodeDestroy(current);
	}
	free(list);
}


llnode *nodeInsert(linkedListPtr l, char *filename){
	int i;

	llnode *node;
	node = l->head;


	for(i = 0; i<l->size; i++){


		if(strcmp(node->filename, filename) == 0){
			node->freq++;
			return node;
		}


		if(node->next != NULL)
		node = node->next;

	}

	llnode *newNode;
	newNode = llnodeCreate(filename);

	node->next = newNode;
	l->size++;

	return newNode;

}


listNode *SLInsert(SortedListPtr l, void *newObj, char *filename){
	int c;

	SortedListPtr list = l;
	if (list->size == 0){
		listNode *newNode = (listNode*)nodeCreate(newObj, NULL);
		

		llnode *node;
		node = llnodeCreate(filename);

		newNode->list->head = node;
		newNode->list->size++;

		list->head = newNode;
		list->size++;

		return newNode;
	}


	listNode *prev = NULL;
	listNode *curr = list->head;

	if((*(list->comp))(newObj, curr->data) < 0){

		listNode *newNode = (listNode*) nodeCreate(newObj, list->head);

		llnode *node;
		node = llnodeCreate(filename);

		newNode->list->head = node;
		newNode->list->size++;

		list->head = newNode;
		list->size++;

		return newNode;
	}

	while (curr != NULL){
		c = (*(list->comp))(newObj, curr->data);
		/*if c=0, the object already exists so do nothing. The function returns 0 because it is done with the insert.*/
		if (c == 0){
			llnode *node;
			node = nodeInsert(curr->list, filename);
			return curr;
		}
		/*the new Node is greater than the current node so the while loop continues.*/
		else if(c > 0){
			prev = curr;
			curr = curr->next;
		}
		/*the new Node is smaller that the current node so insert it in the list after the current node that we are comparing it to.*/
		else if(c < 0){
			listNode *newNode = (listNode*) nodeCreate(newObj, curr);

			llnode *node;
			node = llnodeCreate(filename);

			newNode->list->head = node;
			newNode->list->size++;

			prev->next = newNode;

			newNode->next = curr;
			list->size++;

			return newNode;
		}
	}
	

	listNode *newNode = nodeCreate(newObj, NULL);
	
	llnode *node;
	node = llnodeCreate(filename);
	
	newNode->list->head = node;
	newNode->list->size++;

	prev->next = newNode;
	list->size++;


	return newNode;

}


int SLRemove(SortedListPtr list, void *newObj){
	
	int c;
	listNode *prev = NULL;
	listNode *curr = list->head;
	
	c = (*(list->comp))(newObj, curr->data);

	if(c == 0){
		list->head = curr->next;
		nodeDestroy(curr);
		list->size--;
		return 1;
	}

	prev = curr;
	curr = curr->next;

	while (curr != NULL){
		c = (*(list->comp))(newObj, curr->data);

		if(c == 0){
			if(curr->next == NULL){
				prev->next = NULL;
				nodeDestroy(curr);
				list->size--;
				return 1;
			}
			
			prev->next = curr->next;
			nodeDestroy(curr);
			list->size--;
			return 1;
		}
		else if(c != 0){
			prev = curr;
			curr = curr->next;
		}
	}

	return 0; /* FAILED */

}


SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr SlistIterPtr = (SortedListIteratorPtr) malloc(sizeof (struct SortedListIterator));
	SlistIterPtr->list = list;
	/*returns NULL if the list size is 0. current and next cannot be created.*/
	if (SlistIterPtr->list == 0){
		return NULL;
	}
	SlistIterPtr->current = list->head;

		SlistIterPtr->next = NULL;
	SlistIterPtr->next = list->head->next;

	return SlistIterPtr;
	
}

void SLDestroyIterator(SortedListIteratorPtr iter){
	free(iter);
}

void* SLNextItem(SortedListIteratorPtr iter){
	SortedListIteratorPtr slistIter = iter;
	/*
		if the head of the list is non existent, then this function returns null. MAIN should check if it returns null, if it does then there are no items in the list.
	*/
	if (slistIter->current == NULL){
	
		return NULL;
	}
	listNode *item = slistIter->current;
	/*
		If it is the last item in the list in order to prevent a seg fault we are returning the last value repeatedly.
	*/

	if(item->next == NULL){
		return item;
	}
	slistIter->current = slistIter->next;
	slistIter->next = slistIter->next->next;
	return item;
}



