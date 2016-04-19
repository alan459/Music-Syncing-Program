#include "getLengthField.c"
#include "NetworkHeader.h"
#include "WhoHeader.h"

//#include <stdio.h>
//#include <string.h>
/*
Called when the user enters the 'DIFF' command on the client side
LIST message format 1st 4 chars = message type 'list' next 2 = length, then song:sha.....
*/
void handleDiff(char* receivedList)
{
	receivedList += NUM_BYTES_COMMAND_FIELD; // move pointer past the characters specifiying command type

	// get the length field
	int length = getLength(receivedList);

	// alocate space for the song list to be passed to the database compare methods
	char songList[length][MAX_SONGNAME_LENGTH + SHA_LENGTH + 2];

	// move pointer to start of song:sha pairings
	receivedList += LENGTH_FIELD_BYTES; 
	printf("PTR: %s\n", receivedList);

	// iterate each song/sha and add it to the 2D array to be passed to the compare method in Database.c
	int i;
	for ( i = 0   ;   i  < length   ;  i++ ) 
	{
		printf("ITERATION  : %d\n", i);
		printf("a: PTR: %s\n", receivedList);

		// retrieve song name
		char currentEntry[MAX_SONGNAME_LENGTH + SHA_LENGTH + 2];

		// receivedList pointer should be pointing to start of first song name
		strncpy(currentEntry, receivedList, MAX_SONGNAME_LENGTH);
		printf("b1: PTR: %s\n", receivedList);

		// move pointer to start of SHA for the current song (skipping over the ':' char too)
		receivedList += 5 + 1;
		printf("b2: PTR: %s\n", receivedList);

		// add the colon to the current songList entry
		currentEntry[MAX_SONGNAME_LENGTH] = ':';

		// retrieve the current SHA and add it past the song field and colon
		strncpy(currentEntry + MAX_SONGNAME_LENGTH + 1, receivedList, SHA_LENGTH);
		currentEntry[MAX_SONGNAME_LENGTH + SHA_LENGTH + 1] = 0;

		// add the fully retrieved entry to the song list
		strcpy(songList[i], currentEntry);
		songList[i][MAX_SONGNAME_LENGTH + SHA_LENGTH + 1] = 0;

		// move pointer to start of SHA for the current song (skipping over the ':' char too)
		receivedList += SHA_LENGTH;

		printf("c: PTR: %s\n", receivedList);

	}
	// at this point the song:sha pairings of the packet have been moved to a 2D array to be passed to the database methods for comparison
	printf("ENTRIES:\n");
	for(int i = 0; i < length; i++)
	{
		printf("%s\n", songList[i]);
	}
/*
	char* songs_Not_On_Client = compareSongsToClient(songList, length);
	printf("compareeeee\n");
	char* songs_Not_On_Server = compareSongsToServer(songList, length);

	printf("SONGS NOT ON CLIENT: \n %s \n", songs_Not_On_Client);
	printf("SONGS NOT ON SERVER: \n %s \n", songs_Not_On_Server);


	char a[];*/




}

main() {
	
	char testMessage[BUFFSIZE];
	strcpy(testMessage, "LIST");
	testMessage[4] = 0;
	testMessage[5] = 2;
	char* ptr = testMessage + 6;
	//strcpy(ptr, "Name7:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222221Name1:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name3:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Name2:22221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228UniqC:42221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Na100:12345611111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name3:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Name4:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222229");
	strcpy(ptr, "Name1:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name9:22221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222257");
/*
	open_database("compare.dat");
	printf("past this 2\n");
	printf("past this 3\n");
	close_database();
	printf("%s\n", ptr);
	printf("hello\n");
*/
	//handleDiff();

	handleDiff(testMessage);
	printf("%d\n", MAX_SONGNAME_LENGTH);
	char* aa = "01234567890";
	//aa += MAX_SONGNAME_LENGTH;
	//aa += MAX_SONGNAME_LENGTH;
	//printf("%s\n", aa);
	/*
	char a[5][3];
	strcpy(a[3], "ok");
	strcpy(a[4], "ye");
	printf("%s\n", a[3]);

	printf("%s\n", *(a+3) + 1);*/
}