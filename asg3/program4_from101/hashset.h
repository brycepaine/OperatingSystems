/* Sam Fields (sefields@ucsc.edu) and Taoh Green (tmgreen@ucsc.edu)
 *NAME: hashset.h
 *DESCRIPTION: Header file which defines the 
 *  structures needed for a hashset adt
 *  and exports interfaces for public use.

*/
#ifndef __HASHSET_H__
#define __HASHSET_H__

#include <stdbool.h>

typedef struct hashset hashset;

//
// Create a new hashset with a default number of elements.
//
hashset *new_hashset (int size);

//
// Frees the hashset, and the lists it points at.
//
void free_hashset (hashset* H, int size);

//
// Inserts a new string and library id into the hashset.
//
void put_hashset (hashset* H, char *title, int id);

//
// Looks up the string in the hashset and prints the title
//and libraries that have it
//
void has_hashset (hashset* H, char* title);

//prints the hashset books and library ids useful for debugging
void print_hashset(hashset* H);
#endif

