GCC = gcc -ansi -Wall -Wextra -std=gnu99
GCCC = gcc -c -ansi -Wall -Wextra -std=gnu99
CLEAN = library.o md5.o library fuck
OBJECTS = md5.o library.o
EXECBIN = lib

all: ${EXECBIN}

${EXECBIN}: ${OBJECTS}
	${GCC} -o ${EXECBIN} ${OBJECTS}

md5.o: md5.c md5.h
	${GCCC} md5.c

library.o: library.c
	${GCCC} library.c

clean:
	rm ${CLEAN}
