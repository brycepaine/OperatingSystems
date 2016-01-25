GCC = gcc -ansi -Wall -Wextra -std=gnu99
GCCC = gcc -c -ansi -Wall -Wextra -std=gnu99
CLEAN = md5driver.o md5.o md5driver
OBJECTS = md5.o md5driver.o
EXECBIN = md5driver

all: ${EXECBIN}

${EXECBIN}: ${OBJECTS}
	${GCC} -o ${EXECBIN} ${OBJECTS}

md5.o: md5.c md5.h
	${GCCC} md5.c

md5driver.o: md5.h md5driver.c
	${GCCC} md5driver.c

clean:
	rm ${CLEAN}

all: md5driver
