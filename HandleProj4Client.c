// Kyong Lee

#include "NetworkHeader.h"
#include "WhoHeader.h"

void getSong(char* songName, char* song, int* numBytes);
const char* byte_to_binary(uint8_t x, char* binary);

void HandleProj4Client(int cliSock, char *databaseName)
{
	// printf("reached handleporj1client\n"); \\ debugging
	// receive message from client
	char rcvMessage[BUFFSIZE]; // Buffer for received message
	rcvMessage[0] = '\0'; // initialize to empty C-string
	int retrievedLength = 0; // boolean representing whether we have retrieved value from length field
	unsigned long length_Message = 0; // 2 byte field contains length of message (does not count type field)
	int totalBytesRcvd = 0; // total number of bytes received
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
		strcat(rcvMessage, buffer); // append the received buffer to rcvMessage

		// retrieve the length field from message. (located 4th-5th bytes)
		if (!retrievedLength && numBytesRcvd >= 6)
		{

			char firstBin[9]; char secondBin[9];
			byte_to_binary(rcvMessage[4], firstBin);
			byte_to_binary(rcvMessage[5], secondBin);
			strcat(firstBin, secondBin);
			length_Message = (unsigned long)strtoul(firstBin, NULL, 2);
			printf("length_Message: %lu\n", length_Message); // DEBUGGING

			retrievedLength = 1;
		}

		// update totalBytesRcvd;
		totalBytesRcvd = totalBytesRcvd + numBytesRcvd;

		// if message received is length_Message long, exit the loop
		// 4 for type field, 2 for length field
		if (totalBytesRcvd == 4 + 2 + length_Message)
		{
			break;
		}

	}

	// Check the message type (first 4 bytes in the message)
	char typeField[5];
	strncpy(typeField, rcvMessage, 4); // store type field of received message

	printf("type of first message received: %s\n", typeField); // debugging

	//printf("length: %i\n", (int)strlen(typeField)); // debugging

	// Received LIST message
	// LIST message contains nothing but the message type and length field (which is zero).
	if (strcmp(typeField, LISTType) == 0)
	{
		char listResponse[BUFFSIZE];
		strcpy(listResponse, LISTType);

		// open database file
		open_database(databaseName);
		// retrieve all the songs from the database
		int numEntries; // specifies number of songs
		char** songs = lookup_songs(&numEntries);

		/* debugging
		int q;
		for (q = 0; q < numEntries; q++)
		{
			printf("%s\n", songs[q]);
		}*/

		// find song names and SHA's from database and store in listResponse packet
		int i;
		for (i = 0; i < numEntries; i++)
		{
			char** oneSong = songs+i;
			
			// find the first occurence of ':'
			int k; int firstIndex;
			for (k = 0; k < MAX_SONGNAME_LENGTH+1; k++) // at most MAX_SONGNAME_LENGTH characters before first ':'
			{
				if ((*oneSong)[k] == ':')
				{
					firstIndex = k;
					break;
				}
			}
			
			// retrieve song name
			char songName[MAX_SONGNAME_LENGTH+1];
			strncpy(songName, (*oneSong), firstIndex);
			// append null characters for the rest of songName
			int r;
			for (r = MAX_SONGNAME_LENGTH; r >= firstIndex; r--)
			{
				songName[r] = '\0';
			}
			//printf("songName: %s\n", songName); // debugging

			// retrieve SHA 
			char sha[SHA_LENGTH+1];
			strcpy(sha, (*oneSong)+firstIndex+1);
			//printf("SHA: %s\n", sha); // debugging

			// store song name in listResponse packet
			int j;
			for (j = 0; j < MAX_SONGNAME_LENGTH; j++) // 30 bytes used for song name
			{
				listResponse[4 + 2 + i*(MAX_SONGNAME_LENGTH+SHA_LENGTH) + j] = songName[j]; // 4 bytes for "LIST", 2 bytes for length field
				//printf("character appended: %c\n", userName[j]); // debugging
			}

			// store SHA listResponse packet
			int y;
			for (y = 0; y < SHA_LENGTH; y++) // 128 bytes used for SHA
			{
				listResponse[4 + 2 + i*(MAX_SONGNAME_LENGTH+SHA_LENGTH) + MAX_SONGNAME_LENGTH + y] = sha[y]; // 4 bytes for "LIST", 2 bytes for length field
				printf("character appended: %c\n", sha[y]); // debugging
			}


		}

		/*//debugging
		printf("sent: ");
		int k;
		for (k = 0; k < 4 + 2*(MAX_SONGNAME_LENGTH+SHA_LENGTH); k++) // 4 bytes for LIST and 2*158 bytes for 2 songs
		{
			printf("%c", listResponse[k]);
		}
		printf("\n");*/

		// fill length field in 4th-5th of listResponse packet
		listResponse[5] = (uint16_t)numEntries*(MAX_SONGNAME_LENGTH+SHA_LENGTH);
		listResponse[4] = (uint16_t)numEntries*(MAX_SONGNAME_LENGTH+SHA_LENGTH) >> 8;

		// send listResponse packet
		int length_response = 4 + 2 + numEntries*(MAX_SONGNAME_LENGTH+SHA_LENGTH); // length of listResponse packet
		ssize_t numBytesSent = send(cliSock, listResponse, length_response, 0);
		if (numBytesSent < 0)
		{
			DieWithError("send() failed");
		}

		//close_database(); // close database
	}


	// Received PULL message
	// PULL message contains message type length field (2 bytes), and SHA (128 bytes).
	else if (strcmp(typeField, PULLType) == 0)
	{
		// retrieve SHA from PULL message
		char SHA[SHA_LENGTH+1];
		strncpy(SHA, rcvMessage+6, SHA_LENGTH);
		//printf("SHA RECEIVED: %s\n", SHA); // debugging

		// get the song name corresponding to SHA
		open_database(databaseName); // open database
		char songName[MAX_SONGNAME_LENGTH+1];
		strcpy(songName, getSongName(SHA));
		printf("songName: %s\n", songName); // DEBUGGING 
		
		// get the actual song file from current directory
		char song[MAX_SONG_LENGTH];
		int songSize;
		getSong(songName, song, &songSize);
		printf("SONG SIZE: %i\n", songSize);
		song[songSize-2] = '\0'; // debugging
		printf("SONG RECEIVED: %s\n", song); // debugging
/*
		// create response message
		char pullResponse[MAX_SONG_LENGTH+4+1]; // extra 4 for type field, 1 for newline char
		strcpy(pullResponse, PULLType);
		strncpy(pullResponse, song+4, songSize);
		pullResponse[songSize+4] = '\n'; // append newline character
		
		// send response to client
		ssize_t numBytesSent = send(cliSock, pullResponse, songSize+4+1, 0);
		if (numBytesSent < 0)
		{
			DieWithError("send() failed");
		}*/
	}
}
/*
	// Received PUSH message
	// PULL message contains message type and song file.
	else if (strcmp(typeField, PUSHType) == 0)
	{
		
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
	
}*/

void getSong(char* songName, char* song, int* numBytes)
{
	FILE* filePointer;
	if ( (filePointer = fopen(songName, "r+")) == NULL )
	{
  	fprintf(stderr, "Error: Can't open file %s\n", songName);
  	exit(1);
	}

	int i = 0;
	int c;
	do
	{
    c = getc (filePointer);
    song[i] = c;
		i++;
  } while (c != EOF);

	*numBytes = i;
}
