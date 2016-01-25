/*Taoh Green (tmgreen@ucsc.edu)
 *Bryce Paine (bpaine@ucsc.edu)
 *NAME: List.h
 *DESCRIPTION: Header file which defines the 
 *  structures needed for a generic linked list 
 *  and exports interfaces for public use.
 *IMPLEMENTED AND MODIFIED BY: 
    Taoh Green (tmgreen@ucsc.edu), Sam Fields (sefields@ucsc.edu)
*/

#include <stdio.h>

typedef struct NodeStruct *NodePtr;

typedef struct NodeStruct{
   void* data;
   NodePtr link;
} NodeStruct;

typedef struct ListStruct *ListHndl;/*pointer to ListStruct structs*/

typedef struct ListStruct {
   NodePtr head;
} ListStruct;

NodePtr newNode(void* data);/*creates new Node*/

/*** Constructors-Destructors ***/
ListHndl newList( void );
void insert(ListHndl L, void* data); //post: !isEmpty
void freeList(ListHndl L);/*Pre: L has been created with newList.*/

/* NOTE: all Access functions and Manipulation procedures also have the
 * precondition that L has been initialized by newList().
 * */
/*** Access functions ***/
int isEmpty(ListHndl L);/*returns true if list empty */

/*** print function ***/
void printIntList(FILE* out, ListHndl L);/*prints out the list with the
                                   current element marked, for debugging */
void printStringList(FILE* out, ListHndl L);
