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

	// get the length field
	int length = getLengthField(receivedList);

	// alocate space for the song list to be passed to the database compare methods
	char* songList[length];

	receivedList += LENGTH_FIELD_BYTES; // move pointer to start of song:sha pairings

	// iterate each song/sha and add it to the 2D array to be passed to the compare method in Database.c
	int i;
	for ( i = 0   ;  ( i / (MAX_SONGNAME_LENGTH + SHA_LENGTH) ) < length   ;  i += MAX_SONGNAME_LENGTH + SHA_LENGTH ) 
	{
		// retrieve song name
		char currentEntry[MAX_SONGNAME_LENGTH + SHA_LENGTH + 1];

		strncpy(currentEntry, receivedList + i, MAX_SONGNAME_LENGTH);

		currentEntry[MAX_SONGNAME_LENGTH] = ':';

		// retrieve the current SHA and add it past the song field and colon
		strncpy(currentEntry + MAX_SONGNAME_LENGTH + 1, receivedList + i, SHA_LENGTH);

		// alocate space for a song in the song list to be passed to the database compare methods
		songList[i] = malloc(MAX_SONGNAME_LENGTH + SHA_LENGTH + 2);

		strcpy(songList[i], currentEntry);
		//strcat(songList[i], NULL);

	}

	// at this point the song:sha pairings of the packet have been moved to a 2D array to be passed to the database methods for comparison


	char* songs_Not_On_Client = compareSongsToClient(songList, length);

	char* songs_Not_On_Server = compareSongsToServer(songList, length);

	printf("SONGS NOT ON CLIENT: \n %s \n", songs_Not_On_Client);
	printf("SONGS NOT ON SERVER: \n %s \n", songs_Not_On_Server);


}

main() {
	handleDiff();
}