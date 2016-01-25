/*Taoh Green (tmgreen@ucsc.edu)
 *Sam Fields (sefields@ucsc.edu)
 *NAME: bookFind.c
 *DESCRIPTION: creates a hashset which will store book titles and library ids read 
 *from the first file specified on command line. It then processes the book requests
 *specified in the second file on command line and checks if these books are in
 *the hashset. If they are the library ids will be printed to standard output.
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "hashset.h"


int main(int argc, char **argv) {

	//The first argument is the library file.
    FILE *file = fopen(argv[1], "r");
    if(file==NULL) {
        printf("Need to give program two input files.\n");
        return EXIT_FAILURE;
    }
    char str[300];
    int numBooks;
    int hashSize;
    double testIn;
    if(fgets(str,300,file) != NULL) {
		// scan in the number of books and hashsize
		// testIn is a variable to make sure there are no extra arguments given on
		// the first lin (i.e. bad input)
        int test = sscanf (str, "%d%d%lg", &numBooks, &hashSize, &testIn);
		// only two things should have been scanned
		// if there are more the file is wrong and program ends
        if(test!=2) {
            fprintf(stderr, "bad input\n");
            return EXIT_FAILURE;
        }
    }
    //create the hashset whos size is the size scanned in from file
    hashset *H = new_hashset(hashSize);
    
	//this function is used to remove unneccessary characters from the end
    //of the string
	void remChar(char* str) {
		char *newLine = strchr(str, '\n');
        char *carriageRet = strchr(str, '\r');
		if(newLine !=NULL) {
			*newLine = '\0';
		}
        if(carriageRet != NULL) {
            *carriageRet = '\0';
        }
	}

    //this loop goes through the file and scans in the available books
    //and the libraries that hold them
    for (int i = 0; i < numBooks; ++i) {
        int libId;
        char *scanned;

        char newStr[300];
        //splits the line to obtain library id
		if(fgets(newStr,300,file) != NULL) {
			remChar(newStr);
            scanned = strtok(newStr, ", ");
        }
		sscanf(scanned, "%d", &libId);
        //scans book title
		scanned = strtok(NULL,"");
        //removes whitespace before title
		++scanned;
        //puts word and library id into hashset
		put_hashset(H, scanned, libId);
    }
    fclose(file);
	
    //the second file has  the requests of users
    FILE *secFile = fopen(argv[2], "r");
	if(secFile==NULL) {
        printf("Need to give program two input files.\n");
        return EXIT_FAILURE;
    }
	
	int numReq;
    //scans the number of requests (lines in file)
	 if(fgets(str,300,secFile) != NULL) {
		int test = sscanf(str, "%d%lg", &numReq, &testIn);
		if(test!=1) {
            fprintf(stderr, "bad input\n");
            return EXIT_FAILURE;
        }
	}
    
    
	for (int i = 0; i < numReq; ++i) {
        char str2[300];
        //removes newline character and checks if the book is in hashset
		if(fgets(str2,300,secFile) != NULL) {
			remChar(str2);
			// printf("%s\n", str2);
			has_hashset(H, str2);
		}
	}
   
	fclose(secFile);
    printf("\n");
    //frees the hashset, the lists and nodes
    free_hashset(H, hashSize);

    return EXIT_SUCCESS;
}




