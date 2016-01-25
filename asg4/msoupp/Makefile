# Defines the purpose of the program
.PHONY: all
all: ingest server client materialize

# ------------------- MD5 & INGEST & MATERIALIZE -------------
# ingest = executable file
# ingest.o = object file	
ingest: ingest.o md5.o
	gcc ingest.o md5.o -o ingest 

materialize: materialize.o
	gcc materialize.o md5.o -o materialize 

# ingest.o = object file
# ingest.c = source file
ingest.o: ingest.c
	gcc -c -Wall ingest.c -o ingest.o

materialize.o: materialize.c
	gcc -c -Wall materialize.c -o materialize.o 
	
md5.o: md5.c md5.h
	gcc -c  -Wall md5.c -o md5.o	

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

# -------------------- Cleaner ------------------------------
# Cleans and deletes the object files
# -f argument = force
.PHONY: clean
clean:
	rm -f *.o
