/* 
	Owner: Taoh Green, Bryce Paine, Mariette Souppe
	Class: CMPS 111 Operating Systems
	Assignment 4
	March 15, 2015
	File: ingest.c 
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
	
/* external aditional file */
#include "md5.h"
#include "library.h"

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */
#define PORT_NO		4000	/* Defining port number */

//typedef struct sockaddr SA;

/* Error message function */
void error(char *msg) {
    perror(msg);
    exit(1);
}

static void MDPrint (MD5_CTX *mdContext);
char	keydigest[16];

int main(int argc, char **argv) {
	int					sockfd, n;
	struct sockaddr_in	servaddr;
	struct 				stat fileInfo;
	FILE 				*filepath;
	int 				blockSize = 100;
	char				line[MAXLINE], path[MAXLINE];
	char				buffer[blockSize];
	MD5_CTX 			mdContext;
	FILE 				* fdin;
	FILE 				* fdout;

	/* Check of arg usage */
	if (argc != 3) {
		error("usage: ingest original-file file-receipe\n");
	}

    /*--------------------------------------------------------
    				Establishing socket connection
    --------------------------------------------------------*/

	/* Send out error message if error on socket */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("Socket error\n");
    } 

    /* Write zero-valued bytes to servaddr */
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;			/* Defines protocol*/
	servaddr.sin_port   = htons(5624);		/* Choosing port number */
	
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
    					Ingest stuff
    --------------------------------------------------------*/

	/* Read in the orginal file */
	fdin = fopen(argv[1], "r");
	if (fdin == NULL) {
		error("Error on original-file\n");
	}

	/* Output file for recipe */
	fdout = fopen(argv[2], "w+");
	if (fdout == NULL) {
		error("Error on file-receipe\n");
	}

	/* Obtaining the stats about the file */
	stat(argv[1], &fileInfo);

	/* Obtaining the path of the file */
	filepath = popen("/bin/pwd", "r");
	if (filepath == NULL) {
		error("Error detecting filepath\n");
	}

	/* Obtaining the permission bits of the file */
	int fileChmod = fileInfo.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
	
	while(fgets(line, sizeof(line-1), filepath)) {
		strcat(path, line);
	}
	
	/* File permission, file length, filepath */
	fprintf(fdout,"%o,%d,%s", fileChmod, (int)fileInfo.st_size, path);

	char b[blockSize+100];
	while((n = fread(buffer, blockSize, 1, fdin)) >= 0) {
		/* Converting buffer into md5 key */
		MD5Init (&mdContext);
		MD5Update (&mdContext, buffer, strlen(buffer));
		MD5Final (&mdContext);
		MDPrint (&mdContext);

		//printf ("%s\n", buffer);

		/* Information being sent to the server */
		// printf("n=%d\n", n);
		sprintf(b, "INSERT %s~%s~%d", keydigest, buffer, strlen(buffer));
		write(sockfd, b, sizeof(b));

		//printf("%s\n\n", keydigest);

		/* Sending md5 keys out for the recipe */
		fprintf(fdout, "%s\n", keydigest); 

		/* Response from the server */
		bzero(b, sizeof(b));
		read(sockfd, b, sizeof(b));
		printf("%s\n", b);

		/* Clears out the buffers */
		bzero(keydigest, 16);
		bzero(buffer, blockSize);

		/* Breaks out of loop when nothing to read */
		if(n == 0) break;
	}

	printf("\n");
	fclose(fdin);
	fclose(fdout);
	return 0;
}

static void MDPrint (mdContext)
MD5_CTX *mdContext;
{
	int i;
	char temp[16];

	for (i = 0; i < 16; i++) {
		//printf ("%02x", mdContext->digest[i]);
		sprintf (temp, "%02x", mdContext->digest[i]);
		strcat(keydigest, temp);
	}

}