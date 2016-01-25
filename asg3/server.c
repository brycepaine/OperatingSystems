// Server does the mutexing
// flock & unlock around functions in server

/* 
	Owner: Taoh Green, Bryce Paine, Mariette Souppe
	Class: CMPS 111 Operating Systems
	Assignment 3
	February 27, 2015
	File: server.c 
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

// Hash function library
#include "library.h"

/* Miscellaneous constants */
#define	MAXLINE		4096	/* Max text line length */
#define LISTENQ		1024
#define PORT_NO		4000	/* Defining port number */

/* Error message function for debugging purposes */
void error(char *msg) {
    printf(msg);
	exit(1);
}

int main(int argc, char **argv) {
	int					listenfd, connfd, n;
	struct sockaddr_in	servaddr;
	char				line[MAXLINE+1];			/* Buffer for contents from each line in the file*/
	char				printAll[MAXLINE+1];		/* Buffer for all contents from entire file*/
	char				recvline[MAXLINE + 1];		/* Buffer for input from client */
	char				parseline[MAXLINE + 1];
	char				*i, *g, *p, *d;
	char 				pieces[15][500];
	int					a = 0;
	char 				*msg;

	/* Makes socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Send out error message if error on socket */
	if ( listenfd < 0) {
		error("Socket error\n");
    }
	
	/* Write zero-valued bytes to servaddr */
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family      = AF_INET;				/* Defines protocol */
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	/* Defines socket type _ IPv4 */
	servaddr.sin_port        = htons(4502);			/* Choosing port number */

	/* Bind name on a socket */
	int b = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	printf("%d\n", b);
	
	/* Listen for connection on socket */
	int l = listen(listenfd, LISTENQ);
	printf("%d\n", l);
	
	bzero(recvline, MAXLINE+1);
	bzero(line, MAXLINE+1);
	bzero(printAll, MAXLINE+1);
	
	/* Accept a connection on a socket */
	connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
		
	/* Send out error message if error on accept */
	if (connfd < 0) {
		error("Accept error\n.");
	}
	
	for (;;) {
		/* Read information from the client */
		n = read(connfd, recvline, MAXLINE);

		if ( n < 0) {
			error ("Error on read\n.");
		}
		
		strcpy(parseline, recvline);
		printf("parseline: %s\n", parseline);
		
		if (strcmp(recvline, "exit\n") == 0) {
			shutdown(connfd, 1);
			return 0;
		}
		
		strtok(recvline, " ");
		printf("strtok recvline: %s\n", recvline);
		
		/* Parsing the string */
		if (strcmp(recvline, "INIT") == 0) {
			i = strtok(parseline, "<>/");
			a = 0;
			while ( i != NULL) {
				strcpy(pieces[a],i);
				a++;
				//printf("%s\n", i);
				i = strtok(NULL, "<>/");
			}
			//printf("%s\n", recvline);
			//if ((initialize (pieces[2], atoi(pieces[5]), atoi(pieces[8]))) == -1) {
			if ((pieces[2] || pieces[5] || pieces[8]) == -1) {
				msg = "400 Bad Request";
			} else {
				msg = "200 OK";
			}
			write(connfd, msg, strlen(msg));
			printf("%s\t %s\t %s\n", pieces[2], pieces[5], pieces[8]);
		} 
		
		else if (strcmp(recvline, "GET") == 0) {
			g = strtok(parseline, "<>/");
			a = 0;
			while ( g != NULL) {
				strcpy(pieces[a],g);
				a++;
				//printf("%s\n", i);
				g = strtok(NULL, "<>/");
			}
			int k;
			char d[2000];
			//if (fetch (pieces[2], d, &k) == -1) {
			if (pieces[2] == -1) {
				msg = "400 Bad Request";
			} else {
				msg = "200 OK";
			}
			write(connfd, msg, strlen(msg));
			printf("%s\n", pieces[2]);
			//printf("%s\n", recvline);
		} 
		
		else if (strcmp(recvline, "PUT") == 0) {
			p = strtok(parseline, "<>/");
			a = 0;
			while ( p != NULL) {
				strcpy(pieces[a],p);
				a++;
				p = strtok(NULL, "<>/");
			}
			//if (insert (pieces[2], pieces[5], sizeof(pieces[5])) == -1) {
			if ((pieces[2] || pieces[5]) == -1) {
				msg = "400 Bad Request";
			} else {
				msg = "200 OK";
			}
			msg = "sending message test_INIT";
			write(connfd, msg, strlen(msg));
			printf("%s\t %s\n", pieces[2], pieces[5]);
			//printf("%s\n", recvline);
		} 
		
		else if (strcmp(recvline, "DEL") == 0) {
			d = strtok(parseline, "<>/");
			a = 0;
			while ( d != NULL) {
				strcpy(pieces[a], d);
				a++;
				d = strtok(NULL, "<>/");
			}
			//if (delete(pieces[2]) == -1) {
			if ((pieces[2] || pieces[5]) == -1) {
				msg = "400 Bad Request";
			} else {
				msg = "200 OK";
			}
			write(connfd, msg, strlen(msg));
			printf("%s\t %s\n", pieces[2], pieces[5]);				
			//printf("%s\n", recvline);
		} 
		
		else {
			error("513 LUL WUT?\n");
		}

		/*
		bzero(recvline, MAXLINE+1);
		bzero(line, MAXLINE+1);
		bzero(printAll, MAXLINE+1);		
		bzero(pieces, MAXLINE+1);
		bzero(parseline, MAXLINE+1);
		bzero(msg, MAXLINE+1);
		*/
		
	}
	
	/* Close the connection with the client */
	close(connfd);
}