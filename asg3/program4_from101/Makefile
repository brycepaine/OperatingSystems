#Taoh Green (tmgreen@ucsc.edu)
#Sam Fields (sefields@ucsc.edu)
#NAME: Makefile
#DESCRIPTION: makes the Listdr hashsetdr bookFind executables

all : hashsetdr Listdr bookFind

Listdr : Listdr.o List.o
	gcc -ansi -Wall -Wextra -std=gnu99 -o Listdr Listdr.o List.o

bookFind : bookFind.o hashset.o BookStruct.o List.o
	gcc -ansi -Wall -Wextra -std=gnu99 -o bookFind bookFind.o hashset.o BookStruct.o List.o

hashsetdr : hashsetdr.o hashset.o BookStruct.o List.o
	gcc -ansi -Wall -Wextra -std=gnu99 -o hashsetdr hashsetdr.o hashset.o BookStruct.o List.o

Listdr.o : List.h Listdr.c
	gcc -c -ansi -Wall -Wextra -std=gnu99 Listdr.c

BookStruct.o : BookStruct.h BookStruct.c
	gcc -c -ansi -Wall -Wextra -std=gnu99 BookStruct.c

List.o : List.h List.c
	gcc -c -ansi -Wall -Wextra -std=gnu99 List.c

bookFind.o : hashset.h bookFind.c
	gcc -c -ansi -Wall -Wextra -std=gnu99 bookFind.c

hashsetdr.o : hashset.h hashsetdr.c
	gcc -c -ansi -Wall -Wextra -std=gnu99 hashsetdr.c

hashset.o : List.h BookStruct.h hashset.h hashset.c
	gcc -c -ansi -Wall -Wextra -std=gnu99 hashset.c

clean : rm Listdr Listdr.o List.o hashset.o hashsetdr.o

