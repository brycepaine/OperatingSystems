/*Taoh Green (tmgreen) Sam Fields (sefields
*NAME: Listdr.c
*DESCRIPTION: Used to test the functions of the List.c
*adt.
*/

#include <stdio.h>
#include "List.h"

int main() {
   ListHndl L = newList();
   ListHndl Q = newList();
   insert(L, 25);
   insert(L, 78);
   insert(L, 1231);
   insert(Q, "hello my name is BOB");
   insert(Q, "sam fields");

   //printIntList(stdout, L);
   //printStringList(stdout, Q);
   freeList(L);
   freeList(Q);
}
