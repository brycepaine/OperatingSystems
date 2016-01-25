/*Taoh Green (tmgreen@ucsc.edu), Sam Fields (sefields@ucsc.edu)
*NAME: hashset.c
*DESCRIPTION: Implements the necessary functions for a hashset adt.
*These functions are defined in hashset.h
*
*This adt was originally created in computer science 12b with 
*Wesley Mackey.
*
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "hashset.h"
#include "BookStruct.h"

struct hashset {
   int size;
   ListHndl *chains;
};

unsigned long strhash(char *str) {
   unsigned long hash = 5381;
   int c;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c;    
   return hash;
}

hashset *new_hashset (int size) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = size;
   int sizeof_chains = size * sizeof (ListHndl);//malloc
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   for(int i = 0; i < size; ++i) {
      this->chains[i] = newList();
   }
   return this;
}

void print_hashset (hashset *this) {
   for(int i = 0; i < this->size; ++i) {
      NodePtr curr = this->chains[i]->head;
      while(curr != NULL) {
         printf("%s:  ", getTitle(curr->data));
         printIntList(stdout, getLibraries(curr->data));
         printf("\n");
         curr=curr->link;
      }
   }
}

void put_hashset (hashset *this, char *title, int id) {
   //Make a hashkey for new item
   int hashkey = strhash(title) % this->size;
   NodePtr curr = this->chains[hashkey]->head;
   //Iterate thru chain
   while(curr != NULL) {
      // If word is found, return
        // printf("curr data title: %s\n", getTitle(curr->data));
      if(strcmp(getTitle(curr->data),title) == 0) {
         insert(getLibraries(curr->data), id);
         return;
      }
      curr=curr->link;
   
   } 
   // We've reached NULL. Make a new node with item
   BookPtr book = newBookStruct(title);
   insert(getLibraries(book),id);
   insert(this->chains[hashkey], book);
    //printf("title: %s ID: %d\n", getTitle(book), id);
    // print_hashset(this);
}

void has_hashset (hashset *this, char *title) {
   int has = 0;
   // printf("%s: ", title);
   int hash_index = strhash(title) % this->size;
   NodePtr curr = this->chains[hash_index]->head;
   while(curr != NULL) {
      if(strcmp(getTitle(curr->data), title) == 0) {
         has = 1;
         break;
      }
      curr = curr->link;
   }
   if (has == 0) 
      printf("****not available****\n");
   else {
      printIntList(stdout, getLibraries(curr->data));
      printf("\n");
   }
}

void free_hashset(hashset *this, int size) {
    for (int i = 0; i < size; i++) {
        NodePtr curr = this->chains[i]->head;
        NodePtr tmp;
        while(curr != NULL) {
                tmp = curr;
                curr = curr->link;
                freeList(getLibraries(tmp->data));
                free(getTitle(tmp->data));
                free(tmp->data);
         }
         freeList(this->chains[i]);
    }
    free(this->chains);
    free(this);
}
