Owner: Taoh Green, Bryce Paine, Mariette Souppe
Class: CMPS 111 Operating Systems
Assignment 3
February 27, 2015
File: README.txt

********************GRACE DAYS:********************
***We are using 2 grace days for this assignment***


How to compile program:
	Open two different sessions, one for the client and the server
	make clean
	make
	Start server side and enter the command "server"
	Start the client side and enter the command "client"
	[Connection should be established]
	Client types in commands: INIT, GET, PULL, or DEL
	[Rest of program executes]

File functionality:
PART I
library.h
	This file contains the prototypes of the functions that are
	implemented in lib.c

library.c
	This file contains all of the functions for a hash table,
	such as creating a hash table if one doesn't already exist, 
	inserting a key into the hash table, deleting a key in the 
	hash table, and searching through the hash table for a certain
	key.

PART II
server.c
	The purpose of this file initializes and sets up a connection 
	to connect with the with another host, in our case the client.
	When a connection has been established data is sent between
	the client and server.
	
client.c
	This file is a simple program that makes a connection to another
	host, the server, and read and write data to the host until it
	decides to shut down. 
	
Makefile
	The Makefile creates the executable files and object files that are needed
	to make the program work. Additionally, the Makefile cleans the object
	files after their has been changes to the source file and the program
	is re-executed for testing from new changes on the source file.

design.txt
	The design file describes the purpose of the assignment, goes into detail about
	the data that's used in the program, the operations for the different functions
	that are used in the program, and the algorithm used on the client and server 
	parts of the program.
	
README.txt
	The README file tells a user how to compile the program, the purpose of 
	each file, and any experience that the program had during their process
	of completing their task.

Experience:

	