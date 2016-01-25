/*Sam Fields (sefields@ucsc.edu), Taoh Green (tmgreen@ucsc.edu)
*NAME: BookStruct.c
*DESCRIPTION: The structure contains a ListHndl which will store
*library ids and a char* which will store the title of the book.
*The structure uses the List.c adt, and is used to store books and library
*ids into the hashset.c adt.
*
*/


#include "BookStruct.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct BookStruct{
   ListHndl libraries;
   char *title;
} BookStruct;

BookPtr newBookStruct(char *title) {
   char *newTitle = strdup(title);
   BookPtr new = malloc(sizeof(BookStruct));
   assert(new!=NULL);
   new -> title = newTitle;
   new -> libraries = newList();
   return new;
}

//returns title of bookstruct
//pre: title isnt null
char* getTitle(BookPtr B) {
   return B->title;
}

//returns the list of libraries
//pre: libraries not null
ListHndl getLibraries(BookPtr B) {
   return B->libraries;
}
