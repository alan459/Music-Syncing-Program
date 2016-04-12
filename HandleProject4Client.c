// Kyong Lee

#include "NetworkHeader.h"

const char LISTType[] = "LIST";
const char PULLType[] = "PULL";
const char PUSHType[] = "PUSH";
const char LEAVEType[] = "LEAV";

void HandleProj4Client(int cliSock, char *databaseName)
{
	// printf("reached handleporj1client\n"); \\ debugging
	// receive message from client
	char rcvMessage[BUFFSIZE]; // Buffer for received message
	rcvMessage[0] = '\0'; // initialize to empty C-string
	for (;;)
	{
		//printf("Entered infinite loop\n"); // debugging
		char buffer[BUFFSIZE];
		ssize_t numBytesRcvd = recv(cliSock, buffer, BUFFSIZE-1, 0);
		//printf("numBytesRcvd: %zu\n", numBytesRcvd); // debugging
		//printf("hello buffer received: %s\n", buffer); // debugging
		if (numBytesRcvd < 0)
			DieWithError("recv() failed");
		else if (numBytesRcvd == 0)
			DieWithError("recv() failed: connection closed prematurely");
		buffer[numBytesRcvd] = '\0'; // append null-character
		strcat(rcvMessage, buffer); // append the received buffer to ackMessage

		// if new-line character is received, exit the loop
		if (buffer[numBytesRcvd-1] == '\n')
		{
			//printf("new-line char received\n"); // debugging
			break;
		}
		//printf("end\n"); // debugging
	}

	// Check the message type (first 4 bytes in the message)
	char typeField[5];
	strncpy(typeField, rcvMessage+firstSpacePos+1, secondSpacePos-firstSpacePos-1); // store type field of received message

	//printf("type of first message received: %s\n", typeField); // debugging
	//printf("length: %i\n", (int)strlen(typeField)); // debugging

	// Received LIST message
	if (strcmp(typeField, LISTType) == 0)
	{
		char listResponse[BUFFSIZE];
		strcpy(listResponse, LISTType);

		// open database file
		open_database(databaseName);
		// retrieve all the songs from the database
		int numEntries; // specifies number of users
		char** songs = lookup_song_names(&numEntries);
		strcat(listResponse, ' ');
		
		// find song names and SHA's from database and store in listResponse packet
		int i;
		for (i = 0; i < numEntries; i++)
		{
			char** oneSong = songs+i;
			
			// find the first occurence of ':'
			int k; int firstIndex;
			for (k = 0; k < 31; k++) // at most 30 characters before first ':'
			{
				if ((*oneSong)[k] == ':')
				{
					firstIndex = k;
					break;
				}
			}
			
			// retrieve song name
			char songName[31];
			strncpy(songName, (*oneSong), firstIndex);
			// append null character for the rest of songName
			int r;
			for (r = 30; r >= firstIndex; r--)
			{
				songName[r] = '\0';
			}
			//printf("songName: %s\n", songName); // debugging

			// retrieve SHA 
			char sha[129];
			strcpy(sha, (*oneUser)+firstIndex+1);
			//printf("SHA: %s\n", sha); // debugging

			// store song name in listResponse packet
			int j;
			for (j = 0; j < 30; j++) // 30 bytes used for song name
			{
				listResponse[5+i*158+j] = songName[j]; // 4 bytes for "LIST" and 1 byte for space
				//printf("character appended: %c\n", userName[j]); // debugging
			}

			int y
			// store SHA listResponse packet
			for (y = 0; y < 128; y++) // 30 bytes used for username
			{
				listResponse[5+i*158+30+y] = songName[y]; // 4 bytes for "LIST" and 1 byte for space
				//printf("character appended: %c\n", songName[y]); // debugging
			}


		}

		int listResponseLength = strlen(listResponse); // store length of ackMessage
		listResponse[listResponse] = '\n'; // append new-line character
		listResponse[listResponseLength+1] = '\0'; // append null-character
		ssize_t numBytesSent = send(cliSock, listResponse, strlen(listResponse), 0);
	}



	else // Did not receive LIST message
	{
		//printf("error caused from hello\n"); // debugging
		DieWithError("Invalid type field in client message!");
	}

	// construct ACK message
	char ackMessage[BUFFSIZE];
	strcpy(ackMessage, ACKVersion);
	strcat(ackMessage, ACKType);
	strcat(ackMessage, randString);
	int ackLength = strlen(ackMessage); // store length of ackMessage
	ackMessage[ackLength] = '\n'; // append new-line character
	ackMessage[ackLength+1] = '\0'; // append null-character

	// send ACK message to client
	ssize_t numBytesSent = send(cliSock, ackMessage, strlen(ackMessage), 0);
	if (numBytesSent < 0)
		DieWithError("send() failed");

	// Receive another message from client
	rcvMessage[0] = '\0'; // clear rcvMessage
	for (;;)
	{
		//printf("Entered infinite loop\n"); // debugging
	        char buffer[BUFFSIZE];
		ssize_t numBytesRcvd = recv(cliSock, buffer, BUFFSIZE-1, 0);
		if (numBytesRcvd < 0)
			DieWithError("recv() failed");
		else if (numBytesRcvd == 0)
			DieWithError("recv() failed: connection closed prematurely");
		buffer[numBytesRcvd] = '\0'; // append null-character
		strcat(rcvMessage, buffer); // append the received buffer to ackMessage

		// if new-line character is received, exit the loop
		if (buffer[numBytesRcvd-1] == '\n')
		{
			//printf("new-line char received\n"); // debugging
			break;
		}
		//printf("end\n"); \\ debugging
	}

	// Check if BYE message is received
	firstSpacePos = findnthSpace(rcvMessage, 1); // first space position for rcvMessage
	secondSpacePos = findnthSpace(rcvMessage, 2); // second space position for rcvMessage
	//printf("Second and first space positions: %i : %i\n", (int)secondSpacePos, (int)firstSpacePos); // debugging
	char typeFieldBye[BUFFSIZE]; // clear typeField string
	strncpy(typeFieldBye, rcvMessage+firstSpacePos+1, secondSpacePos-firstSpacePos-1); // store type field of received message
	// start from rcvmessage+firstSpacePos+1 and go for secondSpacePos-firstSpacePos-1

	//printf("type of first message received: %s\n", typeField); // debugging
	//printf("length: %i\n", (int)strlen(typeField)); // debugging

	// Received BYE message
	if (strcmp(typeFieldBye, BYEType) == 0)
	{
		// print BYE message
		puts("Received final client message:");
		fputc('\n', stdout);
		fputs("Client message: ", stdout);
		puts(rcvMessage);
		//printf("length of final message: %i\n", (int)strlen(rcvMessage)); // debugging;
	}
	else // Did not receive BYE message
	{
		DieWithError("Invalid type field in client message!");
	}

	//printf("cliSock closed\n"); // debugging
	close(cliSock); // Close client socket
	
}

/*
 * Given a string and n, returns the position in the string where n'th 
 * space character occurs.
 */
int findnthSpace(const char *str, int n)
{
	int spaceCount = 0;
	int spacePos = 0;
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		// space character is found
		if (str[i] == ' ')
			spaceCount++;
		
		// nth space character is found
		if (spaceCount == n)
		{
			spacePos = i;
			break;
		}
	}

	return spacePos;
}
