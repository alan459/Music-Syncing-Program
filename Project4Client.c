// Kyong Lee

#include "NetworkHeader.h"

#define HELLOVersion "CS332 "
#define HELLOType "HELLO "

#define BYEVersion "CS332 "
#define BYEType "BYE "



/* function declarations */

/*
 * Given a string, returns the position in the string where second space
 * character occurs. Assume there are 2 space characters in the string.
 */
int findSecondSpace(const char *str);

int main (int argc, char *argv[])
{

  /* There are certainly slicker ways of doing this,
   * but I figured there's no need to make the code
   * any more confusing than it has to be at this point.
   */

  // Argument parsing variables
  char *firstName;
  char *lastName;
  char *serverHost = SERVER_HOST;
  char *serverPort = SERVER_PORT; 
  char *servPortString;
  char c;
  int i;

  if ((argc < 5) || (argc > 7)) {
    printf("Error: Usage Project0Client [-s <server IP/Name>[:<port>]] -f <firstName> -l <lastName>\n");
    exit(1);
  }

  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      c = argv[i][1];

      /* found an option, so look at next
       * argument to get the value of 
       * the option */
      switch (c) {
      	case 'f':
			firstName = argv[i+1];
			break;
      	case 'l':
			lastName = argv[i+1];
			break;
      	case 's':
			serverHost = strtok(argv[i+1],":");
			if ((servPortString = strtok(NULL, ":")) != NULL) {
	 		 serverPort = servPortString;
			}
			break;
      	default:
			break;
      }
    }
  }

  /* Your networking code starts here */

	// create a connected TCP socket
	int sock = SetupTCPClientSocket(serverHost, serverPort);
	if (sock < 0)
		DieWithError("SetupTCPClientSocket() failed");

	// construct HELLO message
	char helloMessage[MAXLINE];
	strcpy(helloMessage, HELLOVersion);
	//printf("hello message's first char: %c\n", helloMessage[0]); // debugging
	strcat(helloMessage, HELLOType);
	strcat(helloMessage, firstName);
	strcat(helloMessage, " ");
	strcat(helloMessage, lastName);
	int helloLength = strlen(helloMessage); // store length of helloMessage
	helloMessage[helloLength] = '\n'; // append new-line character
	helloMessage[helloLength+1] = '\0'; // append null-character
	helloLength = strlen(helloMessage); // update helloLength

	//printf("hello-message sent: %s", helloMessage); // debugging
	//printf("helloLength: %d\n", helloLength); // debugging

	// send the HELLO message to the server
	ssize_t numBytes = send(sock, helloMessage, helloLength, 0);
	//printf("numBytes sent for HELLO: %zu\n", numBytes); // debugging
	if (numBytes < 0)
		DieWithError("send() failed");
	else if (numBytes != helloLength)
		DieWithError("send() failed: sent unexpected number of bytes");

	// receive the ACK message
	char ackMessage[MAXLINE];
	ackMessage[0] = '\0'; // initialize to empty C-string
	for (;;)
	{
		//printf("Entered infinite loop\n"); // debugging
		char buffer[BUFFSIZE];
		numBytes = recv(sock, buffer, BUFFSIZE-1, 0);
		if (numBytes < 0)
			DieWithError("recv() failed");
		else if (numBytes == 0)
			DieWithError("recv() failed: connection closed prematurely");
		buffer[numBytes] = '\0'; // append null-character
		strcat(ackMessage, buffer); // append the received buffer to ackMessage

		// if new-line character is received, exit the loop
		if (buffer[numBytes-1] == '\n')
		{
			//printf("new-line char received\n"); // debugging
			break;
		}
		//printf("end\n"); \\ debugging
	}

	// print the ACK message
	fputs("Received ACK message: ", stdout);
	fputs(ackMessage, stdout);

	// retrieve the cookie field from ackMessage
	char cookieString[MAXLINE];
	// position in ackMessage where second space-character occurs
	int secondSpace = findSecondSpace(ackMessage);
	//printf("second-space position: %d\n", secondSpace); // debugging
	// copy the cookie part of ackMessage to cookieString
	strncpy(cookieString, ackMessage+secondSpace+1, strlen(ackMessage)-secondSpace-2);

	// construct BYE message
	char byeMessage[MAXLINE];
	strcpy(byeMessage, BYEVersion);
	strcat(byeMessage, BYEType);
	strcat(byeMessage, cookieString);
	//printf("cookie string: %s\n", cookieString); // debugging
	int byeLength = strlen(byeMessage); // store length of byeMessage
	byeMessage[byeLength] = '\n'; // append new-line character
	byeMessage[byeLength+1] = '\0'; // append null-character
	byeLength = strlen(byeMessage); // update byeLength

	//printf("bye-message sent: %s", byeMessage); // debugging
	//printf("byeLength: %d\n", byeLength); // debugging

	// send the BYE message to the server
	numBytes = send(sock, byeMessage, byeLength, 0);
	if (numBytes < 0)
		DieWithError("send() failed");
	else if (numBytes != byeLength)
		DieWithError("send() failed: sent unexpected number of bytes");

	return 0;
}

/*
 * Given a string, returns the position in the string where second space
 * character occurs. Assume there are 2 space characters in the string.
 */
int findSecondSpace(const char *str)
{
	int spaceCount = 0;
	int spacePos = 0;
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		// space character is found
		if (str[i] == ' ')
			spaceCount++;
		
		// second space character is found
		if (spaceCount == 2)
		{
			spacePos = i;
			break;
		}
	}

	return spacePos;
}

/**
* Takes as a parameter the list of songs/SHAs to compare to our local database contents.
*
* Returns a list of songs/SHAs of files not stored on our local database.
**/
char* compareSongs(char* inputBuffer) 
{
	// allocate a variable for names in the buffer
	char name[30];

	//allocat a variable for sha in the buffer
	char sha[128];

	// generically allocated output buffer
	char result[BUFFSIZE];

	// move input buffer pointer past message type to start of first song
	inputBuffer += 4;

	// go through the entire input buffer
	while(inputBuffer[0] == '\0') 
	{
		// Retrieve next Name 1 character at a time
		strncpy(name, inputBuffer, 30);

		// move input buffer pointer to start of next SHA
		inputBuffer += 30;

		//Retrieve next SHA
		strncpy(sha, inputBuffer, 128);

		// move input buffer pointer past SHA to start of next song
		inputBuffer += 128;

		// if the song is not found in the database, add it to output buffer
		if(containsSong(sha) == 0) 
		{
			strcat(result,name);
			strcat(result,":");
			strcat(result,sha);
			strcat(result,"\n");
		}
	}

	//null terminate the buffer
	strcat(result, "\0");

}

