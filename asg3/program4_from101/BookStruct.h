/*

Sam Fields (sefields) & Taoh Green (tmgreen)

NAME: BookStruct.h

DESCRIPTION: This file defines the functions that are used to create and manipulate
a BookStruct. These functions are specified in BookStruct.c
BookStruct is an ADT that contains a string for a
book's title and a List of libraries that contain the book.

 */

#include "List.h"

typedef struct BookStruct *BookPtr;


/* 
newBookStruct
Precondition: nothing
Postcondition: Returns a pointer a a new BookStruct. The title is set to
               the title given as the argument. The libraries list is set to
	       an empty new list.
 */
BookPtr newBookStruct(char *title);

/*
getTitle
Precondition: nothing
Postcondition: Returns the title string associated with the BookStruct
               pointed at by B.
 */
char* getTitle(BookPtr B);

/*
getLibraries
Precondition: nothing
Postcondition: Returns a pointer to the list of libraries associated
               with the BookStruct pointed at by B.
 */
ListHndl getLibraries(BookPtr B);
