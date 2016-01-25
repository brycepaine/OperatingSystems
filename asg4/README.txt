Owner: Taoh Green, Bryce Paine, Mariette Souppe
Class: CMPS 111 Operating Systems
Assignment 4
March 16, 2015
File: README.txt

********************GRACE DAYS:********************
***We are using 1 grace day for this assignment***

How to compile program:
	Open three different sessions, one for the ingest, materialize, and server
	make clean
	make
	server (on one terminal)
	ingest (on another terminal)
	materialize (on third terminal)
	[Rest of program executes]

	** Note: Program does not support concurrency and base24 encoding
			 However program does recreate the file from the md5 keys
			 and the hash table is persistent.
	
File functionality:
md5.h
	Used the md5 library provided in the assignment to encode our blocks
	of strings into a 32byte encrypted key.

library.h
	This file contains the prototypes of the functions that are
	implemented in lib.c

library.c
	This file contains all of the functions for a hash table,
	such as creating a hash table if one doesn't already exist, 
	inserting a key into the hash table, deleting a key in the 
	hash table, and searching through the hash table for a certain
	key.

server.c
	The purpose of this file initializes and sets up a connection 
	to connect with the with another host, in our case the ingest/materialize.
	When a connection has been established data is sent between
	the ingest/materialize and server.
	
ingest.c
	The purpose of this file is to create a description of a file that 
	allows it be reconstructed from the blocks that are stored
	in the single instance store. And the file will contain the
	protection bits of the file, length of the file, and the original file
	name and the MD5 hash keys. 

materialize.c
	The purpose of this file is to create a file that is equivalent
	to the original file in contents and protections.
	
Makefile
	The Makefile creates the executable files and object files that are needed
	to make the program work. Additionally, the Makefile cleans the object
	files after their has been changes to the source file and the program
	is re-executed for testing from new changes on the source file.

design.txt
	The design file describes the purpose of the assignment, goes into detail about
	the data that's used in the program, the operations for the different functions
	that are used in the program, and the algorithm used on ingest, materialize, and server 
	parts of the program.
	
README.txt
	The README file tells a user how to compile the program, the purpose of 
	each file, and any experience that the program had during their process
	of completing their task.

Experience:
	The program in general was very titius in the sense that you really had
	to pay attention to the details and made sure that data was being sent
	with the write functions. We didn't finish the program unfortunately, 
	but got as much done with the time that the group had. The directions
	were poorly written and it was difficult to understand the exact flow of
	the program. It took longer to understand what the assignment than actually
	code the assignment. Lots of assumptions had to be made in the program which
	made it difficult to put the program together.