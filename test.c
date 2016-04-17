#include <stdio.h>
#include "WhoHeader.h"

#define MAXIMUM_DATABASE_ENTRY_LENGTH 158
#define FIRSTFIELD_LENGTH 30
#define SECONDFIELD_LENGTH 128
#define MAX_NUM_RECORDS 100


static FILE* filePointer;
static char* songList[MAX_NUM_RECORDS];
static char** nullPtr = NULL;
static int* numEntries;

/**
* Takes song name and SHA as a parameter.
* Adds a song and SHA to the end of the file.
* 
* Note: Assumes a new line is at the end of the file so 
* if a non-newline character exists at the end it will result 
* in a combined string.
**/
void addSong(char* songName, char* SHA, char* filename) 
{
	FILE* filePointer;

	if ( (filePointer = fopen(filename,"r+")) == NULL ) 
	{
    	fprintf(stderr, "Error: Can't open file %s\n", filename);
    	exit(1);
  	}

  FILE* append = filePointer;
  fseek(append, 0, SEEK_END); // get top the end of the file

  fprintf(append, "%s:%s\n", songName, SHA); // add to end of file
  fclose(append);
  fclose(filePointer);
}


/**
* Takes as a parameter database name?
* Returns an array containing the song names in the database and their corresponding SHAs
**/
char** lookup_songs (char* filename) {

	FILE* filePointer;

	if ( (filePointer = fopen(filename,"r+")) == NULL ) 
	{
    	fprintf(stderr, "Error: Can't open file %s\n", filename);
    	exit(1);
  	}

  char *currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  char* firstField = malloc(FIRSTFIELD_LENGTH + 1);
  char* secondField = malloc(SECONDFIELD_LENGTH + 1);

  int no_of_entries = 0;

  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) !=  NULL ) 
  {
      firstField = strtok(currentLine, ":");
      //secondField = strtok(NULL, ":")) == NULL;

      if (( secondField = strtok(NULL, ":")) == NULL) {
         break;
      }


      songList[no_of_entries] = (char *) malloc(100);
      sprintf(songList[(no_of_entries)++],"%s:%s", firstField, secondField);

  }
  fclose(filePointer);

  return songList;
}


int main() {
	char* a = "database.dat";
	addSong("newsong", "shaCode", a);
	char** songs = lookup_songs("database.dat");
	int numEntries = 6;
	for(int i = 0; i < numEntries; i++) {
		printf("%s", songs[i]);
	}
	printf("%s \n", a);
	return 0;
}


  // Determine length of the first messsage to be sent
  size_t echoStringLen = strlen(randString); 

  // SEND the FIRST string to the server
  ssize_t numBytes = send(cliSock, randString, echoStringLen, 0);

  // CHECK if the SEND was SUCCESSFUL
  if (numBytes < 0)
    DieWithError("send() failed");
  else if (numBytes != echoStringLen)
    DieWithError("send() - sent unexpected number of bytes");


  // Receive the response message (ack) from the server
  fputs("Received: ", stdout); 
   
 // int j;
  char buffer[BUFFSIZE]; // I/O buffer
  /* Receive up to the buffer size (minus 1 to leave space for
  a null terminator) bytes from the sender */

  // RECIEVE the 1ST MESSAGE from server
  numBytes = recv(cliSock, buffer, BUFFSIZE - 1, 0);

  // CHECK if RECIEVE was SUCCESSFUL
  if (numBytes < 0)
    DieWithError("recv() failed");
  else if (numBytes == 0)
    DieWithError("recv(), connection closed prematurely");
  buffer[numBytes] = '\0';

  // PRINT the RECIEVED message 
  fputs(buffer, stdout); 


  // BYE MESSAGE to be sent back to the server
  char message[BUFFSIZE]; 

  strcpy(message, "CS332 BYE "); 

  // extract the cookie from the string containing the cookie
  char* cook = getCookie(buffer); 
  // assuming the string contains 3 words, with the final being the cookie

  // add the cookie to the message
  strcat(message, cook); 
  strcat(message, "\n");

  // LENGTH of the BYE message to be sent back to the server
  size_t messageLength = strlen(message);

  // SEND the BYE message to the server
  ssize_t numBytes2 = send(cliSock, message, messageLength, 0);

  // CHECK if SEND failed
  if (numBytes2 < 0)
    DieWithError("send() failed");
  else if (numBytes2 != messageLength)
    DieWithError("send() - sent unexpected number of bytes");

  //fputc('\n', stdout); // new line to stdout