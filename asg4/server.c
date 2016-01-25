/* 
	Owner: Taoh Green, Bryce Paine, Mariette Souppe
	Class: CMPS 111 Operating Systems
	Assignment 4
	March 15, 2015
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
#include <fcntl.h>

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
	char 				pieces[15][MAXLINE];
	int					a = 0;
	char 				msg[MAXLINE + 1];
	off_t 				index;

	/* Check of arg usage */
	if (argc != 1) {
		error("usage: server\n");
	}

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
	servaddr.sin_port        = htons(5624);			/* Choosing port number */

	/* Bind name on a socket */
	int b = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	printf("%d\n", b);
	if (b < 0) {
		error ("Bind error\n");
	}
	
	/* Listen for connection on socket */
	int l = listen(listenfd, LISTENQ);
	printf("%d\n", l);
	if (l < 0) {
		error ("Listen error\n");
	}
	
	bzero(recvline, MAXLINE+1);
	bzero(line, MAXLINE+1);
	bzero(printAll, MAXLINE+1);
	
	/* Accept a connection on a socket */
	connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
	if (connfd < 0) {
		error("Accept error\n.");
	}
	
	/* Creating the hash table 
	   init(char *file, int length, int size) */
    initialize ("Hashtable.txt", 100, 2000);

    /* Creating the log file */
    int logfile = open("log.txt", O_RDWR | O_CREAT, 00700);

	for (;;) {

		/* Clears the buffer */
        bzero(recvline, MAXLINE);
        
		/* Reads line from stream */
        //fgets(recvline, MAXLINE+1, stdin);

        /* Read information from the client */
		n = read(connfd, recvline, MAXLINE);

		//printf("%d\n", n);
		//printf("recvline: %s\n\n", recvline);
		strcpy(parseline, recvline);
		//printf("parseline: %s\n\n", parseline);
		strtok(recvline, " ");
		//printf("strtok recvline: %s\n\n", recvline);
        //return 0;

		if ( n < 0) {
			error ("Error on read\n");
		}
		//return 0;
		/* Reads the system documentation from the server */
        /*while (recvline[0] == '\0' && recvline[0] != EOF) {
            n = read(listenfd, recvline, MAXLINE);
            printf("%s\n", recvline);
        	bzero(recvline, MAXLINE+1);
        }*/

        /*while ((n = read(listenfd, recvline, MAXLINE)) > 0) {
        	strcpy(parseline, recvline);
        	
        }*/		

		/*return 0;
    	strcpy(parseline, recvline);
		printf("%s\n", recvline);

		strtok(recvline, " ");
		printf("strtok recvline: %s\n", recvline);*/
		
		/* ------------------------ INSERT -------------------------- */
		if (strcmp(recvline, "INSERT") == 0) {
			p = strtok(parseline, "~");
			a = 0;
			while ( p != NULL) {
				strcpy(pieces[a],p);
				printf("pieces[%d]%s\n", a, pieces[a]);
				a++;
				p = strtok(NULL, "~");
			}

			/*  
			pieces[0] = command + md5 key
			pieces[1] = block of dataclea
			pieces[2] = size of block of data 
			*/

			// return 0;
			p = strtok(pieces[0], " ");
			p = strtok(NULL, "");
			printf("p:%s\n", p);

			if (probe(p) != -1) {
			//if ((pieces[2] || pieces[5]) == -1) {
				strcpy(msg, "DUPLICATE\n");
				//msg = "400 Bad Request";
			} else {
				strcpy(msg, "OK\n");

				/* Appending block data into the log */
				index = lseek(logfile, 0, SEEK_END);
				printf("index: %d\n", index);
				printf("%s\n", pieces[1]);
				write(logfile, pieces[1], strlen(pieces[1]));

				/* Inserting md5 key into hash table */
				insert(p, index, atoi(pieces[2]));

				//msg = "200 OK";
			}
			write(connfd, msg, strlen(msg));
			//printf("%s\t %s\n", pieces[2], pieces[5]);
			//printf("%s\n", recvline);
		} 
		/* ------------------------ FETCH -------------------------- */
		else if (strcmp(recvline, "FETCH") == 0) {
			// printf("fetching\n");
			printf("parseline: %s\n", parseline);
			g = strtok(parseline, " ");
			a = 0;
			while ( g != NULL) {
				strcpy(pieces[a], g);
				a++;
				// printf("%s\n", g);
				g = strtok(NULL, " ");
			}
			int length;
			off_t byte_offset;
			char buf_return[MAXLINE];
			char *md5key;
			md5key = strtok(pieces[1], "\n");
			if (fetch (md5key, &byte_offset, &length) == -1) {
			// if (pieces[2] == -1) {
				strcpy(msg, "NOT FOUND\n");
				//msg = "400 Bad Request";
			} else {
				printf("found\n");
				printf("length: %d\n", length);
				printf("byte_offset: %d\n", (int)byte_offset);
				//msg = "200 OK \n<value>";
				//printf("%s\n", printAll);
				//printf("%s\n", printAll);
				//printf("%s\n", d);
				//printf("%s\n", msg);
				lseek(logfile, byte_offset, SEEK_SET);
				read(logfile, buf_return, length);
				printf("buf_return: %s\n", buf_return);
				sprintf(msg, "FOUND block_length: %d block:~%s\n", length, buf_return);
			}
			bzero(buf_return, MAXLINE);
			bzero(md5key, strlen(md5key));

			// // msg = "test";
			write(connfd, msg, strlen(msg));
			// printf("%s\n", pieces[1]);
			// //printf("%s\n", recvline);
		} 
		/* ------------------------ INQUIRY/ PROBE -------------------------- */
		else if (strcmp(recvline, "INQUIRY") == 0) {
			p = strtok(parseline, "<>/");
			a = 0;
			while ( p != NULL) {
				strcpy(pieces[a],p);
				a++;
				p = strtok(NULL, "<>/");
			}
			if (probe (pieces[2]) == -1) {
			//if ((pieces[2] || pieces[5]) == -1) {
				strcpy(msg, "NOT FOUND\n");
				//msg = "400 Bad Request";
			} else {
				strcpy(msg, "FOUND\n");
				//msg = "200 OK";
			}
			write(connfd, msg, strlen(msg));
			printf("%s\n", pieces[2]);
			//printf("%s\n", recvline);
		} 
		/* ------------------------ ERROR -------------------------- */
		else {
			error("513 LUL WUT?\n");
		}

		/*
		bzero(recvline, MAXLINE+1);
		bzero(pieces, MAXLINE+1);
		bzero(parseline, MAXLINE+1);
		bzero(msg, MAXLINE+1);	
		*/
	}
	
	/* Close the connection with the client */
	close(connfd);
}