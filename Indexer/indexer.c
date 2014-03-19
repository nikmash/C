/*
* index.c
*/

#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <errno.h>
#include     <dirent.h>
#include     <limits.h>
#include     "sorted-list.h"
#include	 "tokenizer.h"
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <unistd.h>


/*Compare strings fucntion just takes in two void* objects and returns the string compare value*/

int compareStrings(void *p1, void *p2)   
{
    char *s1 = p1;
    char *s2 = p2;

    return strcmp(s1, s2);
}

/*This function takes in a file pointer and a sorted list and then prints out into the index 
file that the user specifies*/

int writetoFile(FILE *file, SortedListPtr indexlist){ 
    int j;
    listNode *curr;
    curr = indexlist->head;
    llnode *currnode;

    printf("size is %d \n", indexlist->size);
    for(j = 0; j < indexlist->size; j++){
        printf("here %d\n", j);

        fprintf(file, "<list> %s \n", (char *)curr->data);
        currnode = curr->list->head;
            printf("here here here \n");

        while(currnode != NULL){
            fprintf(file, "%s %d ", currnode->filename, currnode->freq);
            currnode = currnode->next;
        }
        
        fprintf(file, "\n</List> \n");
        curr = curr->next;
        printf("here %d\n", j);

    }

    return 1;
}

/*This function takes in the directory and file name also it takes in the sorted list pointer in 
order to tokenize the file given and also insert the words into the sorted-list*/

void tokenizeFile(char *dir_name, char *filename, SortedListPtr indexlist){
    FILE *fp;

    fp = fopen(dir_name , "r");

    if (fp == NULL)
        fprintf(stderr, "File could not be opened \n");
    

    char line[1000]; 
    char *word;
    TokenizerT *tokenizer;
    int j;

    while(fgets(line, 1000, fp) != NULL){
        
        j = strlen(line);

        tokenizer = TKCreate(line);
        word = TKGetNextToken(tokenizer);
        

            while(word != NULL){
                
                SLInsert(indexlist, word, filename);
                word = TKGetNextToken(tokenizer);
            }

    }

    TKDestroy(tokenizer);
    fclose(fp);

}

/*This function is a loop that is used when a directory is given so in this we are able to check every file and directory
inside the given directory*/

int opendirectory(char *directory, SortedListPtr indexlist){

	struct dirent *dent;
	DIR *dir;
	DIR *newdir;
	dir = opendir(directory);
	char *name;
    char temp[1024];


	while ((dent = readdir(dir)) != NULL){

        name = dent->d_name;

        if (strcmp(name, ".") == 0)
            continue;
        if (strcmp(name, "..") == 0)
            continue;
        if (strcmp(name, ".DS_Store") == 0)
            continue;

        char cwd[1024];
        
        getcwd(cwd, sizeof(cwd));
        
        strcpy(temp, directory);
        strcat(temp, "/");
        strcat(temp, name);

        newdir = opendir(temp);
        
        if (newdir != NULL){
        	opendirectory(temp, indexlist);
        }
        
        if(dent->d_type == DT_REG){
            
            strcpy(temp, directory);
            strcat(temp, "/");
            strcat(temp, name);

            tokenizeFile(temp, name, indexlist);
        }

	}
    return 0;
}

int main (int c, char *v[]) {
    struct dirent *pDirent;
    DIR *pDir;
    DIR *newdir;
    char *currentdir;
    char* name;
    char cwd[1024];


    currentdir = v[2];

    int(*CF)(void*, void*);
    CF = compareStrings;

    SortedListPtr indexlist;
    indexlist = SLCreate(CF);

    if (c < 2 || c > 3) {
        printf ("Usage: testprog <dirname or file name> <index file name>\n");
        return 1;
    }

    pDir = opendir (v[2]);



        while ((pDirent = readdir(pDir)) != NULL) {

            name = pDirent->d_name;
        
            if (strcmp(name, ".") == 0)
                continue;
            if (strcmp(name, "..") == 0)
                continue;
            if (strcmp(name, ".DS_Store") == 0)
                continue;
       
        
            getcwd(cwd, sizeof(cwd));
       
            strcat(cwd, "/");
            strcat(cwd,currentdir);
            strcat(cwd,"/");
            strcat(cwd, name);

            newdir = opendir(cwd);

            if (newdir == NULL)        	
                tokenizeFile(cwd, name, indexlist);
        
            if (newdir != NULL)
       		   opendirectory(cwd, indexlist);

        }

    
    char *indexfile;
    indexfile = v[1];
    char input;
    
    FILE *indexfp;

    if((indexfp = fopen(indexfile, "r")) == NULL){
        indexfp = fopen(indexfile, "w");
        writetoFile(indexfp, indexlist);
    }

    else{
        printf("This file already exists, would you like to overwrite it? ");
        printf("Enter Y or N \n");
        input = getc(stdin);

        if (input == 'Y' || input == 'y'){
            indexfp = fopen(indexfile, "w+");
            writetoFile(indexfp, indexlist);
        }
        if (input == 'N' || input == 'n'){
            printf("Please close the program and restart with a file name that you would like that does not already exist/ you do not want to overwrite it ");
        }
    }

    fclose(indexfp);
    SLDestroy(indexlist);

    closedir (pDir);
    return 0;

}