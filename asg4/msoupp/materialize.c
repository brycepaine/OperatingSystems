/* 
	Owner: Taoh Green, Bryce Paine, Mariette Souppe
	Class: CMPS 111 Operating Systems
	Assignment 4
	March 15, 2015
	File: materialize.c 
*/
// Standard libraries	
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
// Socket libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Miscellaneous constants */
#define	MAXLINE		4096

// #include "library.h"

/* Error message function for debugging purposes */
void error(char *msg) {
    printf(msg);
	exit(1);
}

int main(int argc, char **argv) {
	int					sockfd, n;
	struct sockaddr_in	servaddr;
	FILE 				* recipe;
	char               *rnull;
	char                b[40];
	int fdout;
	/*materialize*/
	char line[MAXLINE+1];
    size_t len = 0;
    //ssize_t read;
	if (argc != 3) {
		error("usage: materialize file-recipe recreated-file\n");
	}
	 /*--------------------------------------------------------
    				Establishing socket connection
    --------------------------------------------------------*/
	/* Send out error message if error on socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("Socket error\n");
    } 
    /* Write zero-valued bytes to servaddr */
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;			/* Defines protocol*/
	servaddr.sin_port   = htons(5623);		/*Choosing port number*/ 
	
	int i = inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	printf("%d\n", i);
	if ( i <= 0) {
		error("Inet_pton error\n");
	}
	
	/* Sends out error message if can't make connection on socket */
	int c = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	printf("%d\n", c);
	if ( c < 0) {
		error("Connect error\n");
	} 
	/*--------------------------------------------------------
    					Materialize stuff
    --------------------------------------------------------*/
    /* Open recipe file */
    recipe = fopen(argv[1], "r");
    if (recipe == NULL) {
		error("Error on recipe-file\n");
	}
	char * parsed;
	if (fgets(line, MAXLINE, recipe) == NULL) {
		error("Error on reading info for output file\n");
		printf( "%s\n", line);
	}
	char *p;
	char pieces[3][MAXLINE];
	char parseline[MAXLINE + 1];
	/* Parsing first like in the original file */

	strcpy(parseline, line);
	p = strtok(parseline, ",");
	int a = 0;
	while ( p != NULL) {
		strcpy(pieces[a],p);
		a++;
		p = strtok(NULL, ",");
	}
	printf("%s-%s-%s\n", pieces[0], pieces[1], pieces[2]);
	/* pieces[0] = permission, 
	   pieces[1] = file size, 
	   pieces[2] = path
	*/

	char mode[10];
	strcpy(mode, pieces[0]);
	int filesize = atoi(pieces[1]);
	// return 0;
	char *path;
	path = strtok(pieces[2], "\n");
	// return 0;

	//sprintf(path, "/%s", argv[2]);
	strcat(path, "/");
	strcat(path, argv[2]);
	printf("%s\n", path);
	// return 0;
	fdout = open(path, O_CREAT | O_RDWR, 00700);
	if (fdout < 0) {
		error("Error on creating recreated-file\n");
	}
	int m = strtol(mode, 0, 8);
	if(chmod(path, m) < 0) {
		error("Error on setting permission and path for file\n");
	}
	// return 0;


	bzero (line, MAXLINE);
	
	while (fgets(line, MAXLINE, recipe) != NULL) {
		rnull = strtok(line, "\n");
		sprintf(b, "FETCH %s", rnull);
		write(sockfd, b, strlen(b));
		bzero(b, strlen(b));
		read(sockfd, b, strlen(b));
		printf("%s\n", b);
		// printf("%s",rnull);
		bzero(rnull, strlen(rnull));
		bzero (line, MAXLINE);
	}

	return 0;
}