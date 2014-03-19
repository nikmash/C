/*
* Cache.c
*/

#include "cache.h"
#include "tokenizer.h"
#include <limits.h>

int var = 0;
int var2 = 0;
int start = 0;
int counter;
int lwsize = 0;

int onoff = 0;
word* listwords[2000];


struct data_{
	int currmemory;
	int maxmemory;
	char *file;

};
typedef struct data_ data;

/* Creates a LinkedList with the head node and size initialized to Null and 0 */

linkedListPtr LLCreate(){
	linkedListPtr list = (linkedListPtr) malloc(sizeof(struct linkedlist));
	list->head = NULL;
	list->size = 0;

	return list;
}

/* Inserts a node into a Linked List, the node is created with the token that is passed in as a paramter. */

int LLInsert(linkedListPtr list, char *token){
	llnode *node = (llnode *) malloc(sizeof(struct llnode_));
	node->filename = token;
	node->next = NULL;

	if (list->size == 0){
		list->head = node;
		list->size++;
		return 1;
	}


	else {
		llnode *curr = list->head;
		llnode *prev = NULL;
		while(curr != NULL){
			if(strcmp(curr->filename, token) == 0){
				return 0;
			}
			prev = curr;
			curr = curr->next;
		}

		prev->next = node;
		list->size++;
		return 1;
	}


	return 0;
}

/* A Node is freed in memory. */

void nodeDestroy(llnode *node){
	if(node == NULL){
		return;
	}
	free(node);

}

/* This function is specifically for the "And" search query. When a filename is not found in common between words,
	it needs to be removed from the linked list of result(output) filenames.*/

int LLRemove(linkedListPtr list, char *token){
	llnode *curr = list->head;
	llnode *prev = NULL;


	/*INFINITE LOOP*/
	while (curr != NULL){

		if(strcmp(curr->filename, token) == 0){
			if(strcmp(curr->filename, list->head->filename) == 0){
				list->head = curr->next;
				list->size--;
				nodeDestroy(curr);
				return 1;
			}
			else if(curr->next == NULL){
				prev->next = NULL;
				nodeDestroy(curr);
				list->size--;
				return 1;
			}
			else{
				prev->next = curr->next;

				nodeDestroy(curr);

				list->size--;
				return 1;
			}
		}

		prev = curr;
		curr = curr->next;
	}



	return 0; /* FAILED TO REMOVE */
}



void LLDestroy(linkedListPtr list){
	llnode *curr = list->head;
	llnode *temp = NULL;


	while(curr != NULL){
		temp = curr->next;

		curr->next = NULL;
		free(curr);
		
		curr = temp;
	}

	free(list);
}

/* This function is used to check whether, while reading the indexed file, 
	we are at the beginning of a list of filenames for a specific word or
	we are at the end. */

int checktoken(char *token){

	
	if(strcmp(token, "<list>") == 0){
		var = 1;
		counter = 0;
	}
	if(strcmp(token, "</list>") == 0){
		var2 = 1;
		start = 0;
	}

	return 1;
}

/* Least Recently Used algorithm */

int LRU(char *wordc, data *dataptr, linkedListPtr searchlist){
	FILE *fp;
	fp = fopen(dataptr->file, "r");


	char *token;
	TokenizerT *tokenizer;
	char line[1000];

	int memusage = 0;
	int counter = 0;

	word *newword;

	int on = 0;


	while(fgets(line, 1000, fp) != NULL){

			tokenizer = TKCreate(line);
			token = TKGetNextToken(tokenizer);


			while (token != NULL){
				
				if(on == 1){
					if(strcmp(token, "</list>") == 0){
						on = 2;
						break;
					}

					else if(counter % 2 == 0){
						LLInsert(newword->filelist, token);
						memusage = memusage + strlen(token);
					}
					counter++;
				}



				if(strcmp(token, wordc) == 0){
					

					newword = (word *)malloc(sizeof(struct words));
					newword->counter = 1;
					newword->string = token;
					newword->filelist = LLCreate();
					counter = 2;
					memusage = memusage + strlen(token);
					on = 1;


				}

				token = TKGetNextToken(tokenizer);		
			}
			if(on == 2){
				break;
			}
			
	}


	if(counter == 0){
		printf("WORD %s DOES NOT EXIST \n", wordc);
		return -1;
	}

	newword->memory = memusage;

	llnode *currsearch = searchlist->head;
	llnode *prevsearch = NULL;


	//int count = 0;
	int lowest = INT_MAX;
	int lowestindex;
	int i;
	int retval;

	on = 0;


	fclose(fp);

	if(dataptr->maxmemory-dataptr->currmemory > memusage){
		listwords[lwsize] = newword;
		dataptr->currmemory = dataptr->currmemory + memusage;
		lwsize++;
	}

	else{
		while(dataptr->maxmemory-dataptr->currmemory < memusage){
			for(i = 0; i<lwsize; i++){
				while(currsearch != NULL){

					if(strcmp(currsearch->filename, listwords[i]->string)==0){
						on = 1;
						break;
					}
						

					prevsearch = currsearch;
					currsearch = currsearch->next;

				}


				if(on == 0){

					if(listwords[i]->counter < lowest){
						lowest = listwords[i]->counter;
						lowestindex = i;
					}

				}
			}


			dataptr->currmemory = dataptr->currmemory - listwords[lowestindex]->memory;
			free(listwords[lowestindex]);


			if(dataptr->maxmemory-dataptr->currmemory > memusage){
				listwords[lowestindex] = newword;
				dataptr->currmemory = dataptr->currmemory + memusage;
				retval = lowestindex;
				break;
			}

			else{
				if(lowestindex != lwsize-1){
					for(i = lowestindex; i<lwsize-1; i++){
						listwords[lowestindex] = listwords[lowestindex + 1];
						listwords[lowestindex+1] = NULL;
					}
					lwsize--;
				}
				
			}

		/*	if(count == 0){
				printf("WHAT IS THIS %d \n", lowestindex);
				printf("whats in 0 %s \n", listwords[lowestindex]->string);

				dataptr->currmemory = dataptr->currmemory - listwords[lowestindex]->memory;

				free(listwords[lowestindex]);
				printf("HERE1 \n");

				listwords[lowestindex] = newword;
				dataptr->currmemory = dataptr->currmemory + memusage;
				retval = lowestindex;
				count++;

			}
			else{
				printf("WHAT IS THIS 2 %d \n", lowestindex);

				dataptr->currmemory = dataptr->currmemory - listwords[lowestindex]->memory;

				free(listwords[lowestindex]);
				if(lowestindex != lwsize-1){
					for(i = lowestindex; i<lwsize; i++){
						listwords[lowestindex] = listwords[lowestindex + 1];
						listwords[lowestindex + 1] = NULL;
					}
					lwsize--;
				}
			}

			*/
		}
	}

	

	return retval;
}

/* "Or" Search function. Takes in an array of the indexed words, size of that array,
	a Linked List that will be used to store the result(output) of the search,
	and a Linked List of the input words that the function should search for. */

int searchor(linkedListPtr foundlist, linkedListPtr searchlist, data *dataptr){
	int y;
	int comp;
	int boolean;
	boolean = 1;
	int lru;

	/* use this loop for however many words are input by the user*/
	llnode *currsearch = searchlist->head;
	
	while(currsearch != NULL){
		

		for (y = 0; y < lwsize; y++){
			comp = strcmp(currsearch->filename, listwords[y]->string);

			if (comp == 0){
				listwords[y]->counter++;
				llnode *curr = listwords[y]->filelist->head;
				llnode *prev = NULL;

				while(curr != NULL){
						LLInsert(foundlist, curr->filename);
						prev = curr;
						curr = curr->next;
				}
				break;
			}
		}

		if(comp != 0){
			printf("Could not find %s in cache \n", currsearch->filename);
			lru = LRU(currsearch->filename, dataptr, searchlist);

			if(lru == -1){
				printf("Could not find %s in specified text file \n", currsearch->filename);
				return 0;
			}
			continue;
		}
		currsearch = currsearch->next;	
	}

	if(foundlist->head == NULL){
		fprintf(stderr, "The words you entered are not in the index file you specified \n");
		return 0;
	}
	


	llnode *currfoundit = foundlist->head;
	while(currfoundit != NULL){

		printf("%s \n", currfoundit->filename);
		currfoundit = currfoundit->next;
	}
	return 1;
}

/* "And" Search function. Takes in an array of the indexed words, size of that array,
	a Linked List that will be used to store the result(output) of the search,
	and a Linked List of the input words that the function should search for. */


int searchand(linkedListPtr foundlist, linkedListPtr searchlist, data *dataptr){
	int y;
	int w;
	int boolean;
	boolean = 1;
	int on = 0;
	int lru;


	/*THIS FIRST FOR LOOP HAS TO PUT THE FILES OF THE FIRST WORD INTO THE FOUND ARRAY */

	llnode *currsearch = searchlist->head;

	while(on == 0){
		for (y = 0; y < lwsize; y++){
			w = strcmp(currsearch->filename, listwords[y]->string);
			if (w == 0){
				listwords[y]->counter++;
				llnode *curr = listwords[y]->filelist->head;
				llnode *prev = NULL;
				while(curr != NULL){
					LLInsert(foundlist, curr->filename);
					prev = curr;
					curr = curr->next;	
				}
				on = 1;
				break;
			}
		}
		if(w != 0){
			printf("Word was not in cache %s \n", currsearch->filename);
			lru = LRU(currsearch->filename, dataptr, searchlist);

			if(lru == -1){
				printf("Could not find %s in specified text file \n", currsearch->filename);
				return 0;
			}
		}
	}

	currsearch = currsearch->next;
	
	/* THIS SECOND FOR LOOP GETS ALL THE OTHER WORDS AND THEIR FILES AND COMPARES THEM TO WHATEVER IS ALREADY IN THE FOUND ARRAY.
	EACH FILE IN THE FOUND ARRAY IS COMPARED TO THE OTHER WORDS' FILES, AND IF IT DOESNT EXIST THEN THAT FILE IS DELETED 
	FROM THE FOUND ARRAY */
	int found = 0;
	
	while(currsearch != NULL){
		for (y = 0; y < lwsize; y++){
			w = strcmp(currsearch->filename, listwords[y]->string);

			if (w == 0){
				found = 1;
				listwords[y]->counter++;
				llnode *curr = listwords[y]->filelist->head;
				llnode *prev = NULL;
				llnode *currfound = foundlist->head;


				while(currfound != NULL){
						curr = listwords[y]->filelist->head;

					while(curr != NULL){
							boolean = strcmp(currfound->filename, curr->filename);
							if (boolean == 0){
								break;
							}
							prev = curr;
							curr = curr->next;
					}
					
					if(boolean != 0){
						LLRemove(foundlist, currfound->filename);
					}

					currfound = currfound->next;
					boolean = 1;
				}
				
			}
		}
		if(found == 0){
			printf("%s was not in cache \n", currsearch->filename);
			lru = LRU(currsearch->filename, dataptr, searchlist);
				if(lru == -1){
					printf("%s is not in the specified file", currsearch->filename);
					return 0;
				}
			continue;
		}

		currsearch = currsearch->next;
	}

	if(foundlist->head == NULL){
		fprintf(stderr, "There were no files with all input words \n");
		return 0;
	}

	llnode *currfoundit = foundlist->head;
	while(currfoundit != NULL){

		printf("%s \n", currfoundit->filename);
		currfoundit = currfoundit->next;
	}

	
	return 0;
}


int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Please enter a index file \n");
		printf("Usage: <name of compiled program> <option> <Memory Size> <Index File> \n");
	}
	
	if(argc == 4){
		
		FILE *fp;
				
		/***** CONVERT INPUTTED STRING TO MEMORY SIZE *******/
		data *dataptr = (data *) malloc(sizeof(struct data_));

		char *rawmemory = argv[2];
		int num = 0;
		char *memory = rawmemory;
		int multiplier;

		while(isdigit(*rawmemory)){
			num++;
			rawmemory++;
		}

		if(*rawmemory == 'K'){
			multiplier = 1024;
		}

		if(*rawmemory == 'M'){
			multiplier = 1024 * 1024;
		}

		if(*rawmemory == 'G'){
			multiplier = 1024 * 1024 * 1024;
		}

		*rawmemory = '\0';
		dataptr->maxmemory = atoi(memory) * multiplier;
		dataptr->currmemory = 0;


		/**************************************************/

		char line[1000];
		TokenizerT *tokenizer;
		char *token;

		word *newword;
		int memusage = 0;

		dataptr->file = argv[3];

		fp = fopen(argv[3], "r");
		if(fp == NULL){
			fprintf(stderr, "file could not be opened \n");
		}

		while(fgets(line, 1000, fp) != NULL){
			tokenizer = TKCreate(line);
			token = TKGetNextToken(tokenizer);

			while (token != NULL){
				checktoken(token);
				

				if(start == 1){
					if (!(counter % 2)){
						/* THIS IS WHERE I GET THE FILENAME STRING OF EACH WORD*/
						LLInsert(newword->filelist, token);
						memusage = memusage + strlen(token);
								
					}

					counter++;
				}


				/*fprintf(fp2, "%s \n", token);*/
						
				token = TKGetNextToken(tokenizer);
						
				if(var == 1){
					newword = (word *)malloc(sizeof(struct words));
					newword->string = token;
					newword->filelist = LLCreate();
					memusage = memusage + strlen(token);

					var = 0;
					start = 1;
					counter++;
				}
				if(var2 == 1){
					/* AFTER I HAVE ADDED ALL THE FILENAME INDEXES TO THE WORD THEN I INSERT IT INTO listwords array and print it out onto the console */
					if(memusage + dataptr->currmemory > dataptr->maxmemory){
						free(newword);
						break;
					}
					else{
						listwords[lwsize] = newword;
						lwsize++;
						var2 = 0;
						dataptr->currmemory = dataptr->currmemory + memusage;
						newword->memory = memusage;
						memusage = 0;
					}
					
				}

			}

			if(memusage + dataptr->currmemory > dataptr->maxmemory)
				break;
		}

		var = 0;
		var2 = 0;
		start = 0;

		TKDestroy(tokenizer);
		fclose(fp);


		char str[100];
		int x;

		
		for(; ;){
			linkedListPtr searchlist = LLCreate();
			linkedListPtr foundlist = LLCreate();

			/* LOOPING USER INTERFACE */
			printf("search> ");
			fgets(str, 100, stdin);

			  

			x = strlen(str)-1;
			if(str[x] == '\n') 
			    str[x] = '\0';

			token = strtok(str, " ");
			
			LLInsert(searchlist, token);

			
			while ((token = (strtok(NULL, " "))) != NULL){
				LLInsert(searchlist, token);
				searchlist->size++;
			}
			
		
			if (strcmp(searchlist->head->filename, "q") == 0){
			  	break;
			}			

			if(strcmp(searchlist->head->filename, "so") == 0){
				searchlist->head = searchlist->head->next;
				searchor(foundlist, searchlist, dataptr);
			}
			else if(strcmp(searchlist->head->filename, "sa") == 0){
				searchlist->head = searchlist->head->next;
				searchand(foundlist, searchlist, dataptr);
			}
			LLDestroy(searchlist);
			LLDestroy(foundlist);

		}

		int i;

		for (i = 0; i < lwsize; i++){
			LLDestroy(listwords[i]->filelist);
			free(listwords[i]->string);
			free(listwords[i]);
		}


	}
	
				
}


