# Owner: Taoh Green, Bryce Paine, Mariette Souppe
# Class: CMPS 111 Operating Systems
# Assignment 3
# February 27, 2015
# File: Makefile

# Defines the purpose of the program; client and server files 
.PHONY: all
all: client server

# ---------------------- Client -----------------------------
# client = executable file
# client.o = object file	
client: client.o
	gcc client.o -o client 

# client.o = object file
# client.c = source file
client.o: client.c
	gcc -c -Wall client.c -o client.o 

# ---------------------- Server -----------------------------
# server = executable file
# server.o = object file
server: server.o library.o
	gcc server.o -o server library.o

# server.o = object file
# server.c = source file
server.o: server.c library.h
	gcc -c -Wall server.c -o server.o 
	
# ---------------------- Library -----------------------------
# library = executable file
# library.o = object file
library.o: library.c
	gcc -c -Wall library.c -o library.o 

# -------------------- Cleaner ------------------------------	
# Cleans and deletes the object files
# rm = remove 
# -f argument = force
# *.o = for all .o(object) files
.PHONY: clean
clean:
	rm -f *.o