#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>
#include <netinet/in.h>

#define SERVER_HOST "141.166.207.144"  /* wallis IP address */
#define SERVER_PORT "31401"
#define VERSION = "CS332"

#define SA struct sockaddr

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	MAXSOCKADDR  128	/* max socket address structure size */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */
#define	LISTENQ		1024	/* 2nd argument to listen() */
#define SHORT_BUFFSIZE  100     /* For messages I know are short */
void DieWithError(char *errorMessage); /*Error handling function */
//void HandleProj1Client(int cliSock, char *randString, char *serverHost, unsigned short serverPort);
void HandleTCPClient(int clntSocket, char cookie[]);
void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);

// Returns the second field in the string
char* getCookie(char cookieString[BUFFSIZE]);