Taoh Green (tmgreen@ucsc.edu)
Sam Fields (sefields@ucsc.edu)

I, Taoh Green, and Sam Fields worked together on this code and 
Taoh Green will be turning in the files.


README file for program Finding Books


The program folder contains the following files:

README.txt:	
        This documentation file

List.h:
        The header file describing the ADT operations

List.c:
        The implementation of the generic linked list ADT  

Listdr.c:
        The driver program for testing the generic linked list ADT;

BookStruct.h:
        the header file for the structure.

BookStruct.c:
        a structure which has a linked list of lib ids and a title of a book

hashset.h:
        The header file for the hashset adt operations

hashset.c:
        The implementation of the hashset ADT.

hashsetdr.c
        the driver program for testing the hashset ADT operations

bookFind.c:
        creates a hashset of books and library ids to be used to lookup
        which libraries have which books. The program reads from files
        specified on the command line

Makefile:
        A makefile that compiles the module and driver
        it makes the object files Listdr.o BookStruct.o List.o bookFind.o
        hashsetdr.o hashset.o, and the executables Listdr, hashsetdr, and 
        bookFind.

