#include "getLengthField.c"
#include "NetworkHeader.h"
#include "WhoHeader.h"

/*
Called when the user enters the 'DIFF' command on the client side
LIST message format 1st 4 chars = message type 'list' next 2 = length, then song:sha.....
*/
void handleDiff(char* receivedList)
{
	receivedList += NUM_BYTES_COMMAND_FIELD; // move pointer past the charqacters specifiying command type
	printf("past this 11\n");
	// get the length field
	int length = getLength(receivedList);
	printf("past this 12 length %d \n", length);
	// alocate space for the song list to be passed to the database compare methods
	char* songList[length];
	printf("past this 13\n");
	receivedList += LENGTH_FIELD_BYTES; // move pointer to start of song:sha pairings
printf("past this 14\n");
	// iterate each song/sha and add it to the 2D array to be passed to the compare method in Database.c
	int i;
	for ( i = 0   ;   i  < length   ;  i++ ) 
	{
		printf("ITERATION  : %d\n", i);
		// retrieve song name
		char currentEntry[MAX_SONGNAME_LENGTH + SHA_LENGTH + 2];

//printf("past this 15\n");
		strncpy(currentEntry, receivedList + (i * (MAX_SONGNAME_LENGTH + SHA_LENGTH + 1)), MAX_SONGNAME_LENGTH);

//printf("past this 16\n");
		currentEntry[MAX_SONGNAME_LENGTH] = ':';

		// retrieve the current SHA and add it past the song field and colon
		strncpy(currentEntry + MAX_SONGNAME_LENGTH + 1, receivedList + (i * (MAX_SONGNAME_LENGTH + SHA_LENGTH + 1) + MAX_SONGNAME_LENGTH + 1), SHA_LENGTH);

		// alocate space for a song in the song list to be passed to the database compare methods
		songList[i] = malloc(MAX_SONGNAME_LENGTH + SHA_LENGTH + 2);

		strcpy(songList[i], currentEntry);
		//strcat(songList[i], NULL);

	}
	printf("OUTAAA HERE\n");
	// at this point the song:sha pairings of the packet have been moved to a 2D array to be passed to the database methods for comparison


	char* songs_Not_On_Client = compareSongsToClient(songList, length);
	printf("compareeeee\n");
	char* songs_Not_On_Server = compareSongsToServer(songList, length);

	printf("SONGS NOT ON CLIENT: \n %s \n", songs_Not_On_Client);
	printf("SONGS NOT ON SERVER: \n %s \n", songs_Not_On_Server);


}

main() {
	char testMessage[BUFFSIZE];
	strcpy(testMessage, "LIST");
	testMessage[4] = 0;
	testMessage[5] = 2;
	char* ptr = testMessage + 6;
	//strcpy(ptr, "Name7:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222221Name1:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name3:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Name2:22221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228UniqC:42221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Na100:12345611111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name3:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Name4:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222229");
	strcpy(ptr, "Name1:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name9:22221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222257");

	printf("past this 1\n");

	open_database("compare.dat");
	printf("past this 2\n");
	handleDiff(testMessage);
	printf("past this 3\n");
	close_database();
	printf("%s\n", ptr);
	printf("hello\n");

	//handleDiff();
}