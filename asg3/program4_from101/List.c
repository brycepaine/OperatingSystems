/*Bryce Paine (bpaine@ucsc.edu)
 *Taoh Green (tmgreen@ucsc.edu)
 *NAME: List.c
 *DESCRIPTION: Implements the necessary functions in 
 *  the generic linked List.These functions are defined 
 *  in List.h 
    IMPLEMENTED AND MODIFIED BY: 
    Taoh Green (tmgreen@ucsc.edu), Sam Fields (sefields@ucsc.edu)

*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

/*creates new ListHndl L. sets
 * head pointer to NULL*/
ListHndl newList(void){
   ListHndl L;
   L = malloc (sizeof(ListStruct));
   assert(L!=NULL);
   L->head = NULL;
   return L;
}

/*creates newNode of type NodeStruct.
 * Assigns data and sets link pointers
 * to NULL*/
NodePtr newNode (void* data) {
   NodePtr tmp = malloc(sizeof(NodeStruct));
   assert(tmp!=NULL);
   tmp->data = data;
   tmp->link=NULL;
   return tmp;
}

/*returns true if list is empty*/
int isEmpty(ListHndl L){
   return(L->head==NULL);
} 

/*Inserts a new element at front of list
 *Post: !isEmpty();  */
void insert(ListHndl L, void* data){
   NodePtr new = newNode(data);
   if(L->head == NULL){
      L->head = new;
   }
   else{
      new->link = L->head;
      L->head = new;
   }
   assert(!isEmpty(L));
}

/*frees List by deleting last node. last will eventually
 *point to first so all nodes will be freed.  Then frees 
 *the ListHndl.  Pre: L has been created with newList*/
void freeList(ListHndl L){
   assert(L!=NULL);
   NodePtr tmp;
   while(L->head != NULL){
      tmp = L->head;
      L->head = L->head->link;
      free(tmp);
   }
   free(L);
}


/*prints out the list integer */
void printIntList(FILE* out, ListHndl L) {
   NodePtr tmp = L->head;
   while(tmp != NULL) {
      //if(tmp == L->current) printf("curr: ");
      fprintf(out, "%d  ", tmp->data);
      tmp = tmp->link;
   }
   free(tmp);
}
/*prints out the list string */
/*void printStringList(FILE* out, ListHndl L) {
   NodePtr tmp = L->head;
   while(tmp != NULL) {
      //if(tmp == L->current) printf("curr: ");
      fprintf(out, " %s\n", tmp->data);
      tmp = tmp->link;
   }
   free(tmp);
}*/

