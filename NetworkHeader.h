// Kyong Lee

#ifndef NETWORK_HEADER_H 
#define NETWORK_HEADER_H

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>
#include <stdint.h>			/* for uint16_t */
#include <inttypes.h>		/* for printing uint16_t */

#define SERVER_HOST "mathcs02"  /* wallis IP address */
#define SERVER_PORT "31305"

#define SA struct sockaddr

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	MAXSOCKADDR  128	/* max socket address structure size */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */
#define	LISTENQ		1024	/* 2nd argument to listen() */
#define SHORT_BUFFSIZE  100     /* For messages I know are short */
#define MAX_SONGNAME_LENGTH 255 /* max song name length */
#define SHA_LENGTH 128 /* length of SHA-512 */
#define MAX_SONG_LENGTH 65535 /* max length of song */

#define NUM_BYTES_COMMAND_FIELD 4
#define LENGTH_FIELD_BYTES 2

#define LISTType "LIST"
#define DIFFType "DIFF"
#define PULLType "PULL"
#define PUSHType "PUSH"
#define LEAVEType "LEAV"

void DieWithError(const char *errorMessage); /*Error handling function */
void HandleProj4Client(int cliSock, char *databaseName);
int SetupTCPClientSocket(const char *host, const char *service);


#endif
