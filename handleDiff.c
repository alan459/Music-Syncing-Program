#include "getLengthField.c"
#include "NetworkHeader.h"
#include "WhoHeader.h"

/*
NOTE - Passed 2/2 test cases, seems to be working [many of the problems when testing were apprently caused by forgetting to change
the length field to the corresponding number of entries in the input buffer].
*/


 
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
	char* songList[length];//[MAX_SONGNAME_LENGTH + SHA_LENGTH + 2];
	// ** CAN'T do a double array need to use malloc() swince can't pass 2D array to function unless pointer is involved

	// move pointer to start of song:sha pairings
	receivedList += LENGTH_FIELD_BYTES; 
	printf("PTR: %s\n", receivedList);

	// iterate each song/sha and add it to the 2D array to be passed to the compare method in Database.c
	int i;
	for ( i = 0   ;   i  < length   ;  i++ ) 
	{
		// necessary instead of intializing as 2D array because function call won't work otherwise 
		songList[i] = (char*) malloc(MAX_SONGNAME_LENGTH + SHA_LENGTH + 2);

		// retrieve song name
		char currentEntry[MAX_SONGNAME_LENGTH + SHA_LENGTH + 2];


		// receivedList pointer should be pointing to start of first song name
		strncpy(currentEntry, receivedList, MAX_SONGNAME_LENGTH);


		// move pointer to start of SHA for the current song (skipping over the ':' char too)
		receivedList += MAX_SONGNAME_LENGTH + 1;

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

	}
	// at this point the song:sha pairings of the packet have been moved to a 2D array to be passed to the database methods for comparison
	
	/*printf("ENTRIES:\n"); // DEBUGGING
	for(int i = 0; i < length; i++)
	{
		printf("%s\n", songList[i]);
	}*/

	char* songs_Not_On_Client = compareSongsToClient(songList, length);
	printf("SONGS NOT ON CLIENT:\n%s \n", songs_Not_On_Client);

	char* songs_Not_On_Server = compareSongsToServer(songList, length);
	printf("SONGS NOT ON SERVER:\n%s \n", songs_Not_On_Server);





}
/*
main() {
	
	char testMessage[BUFFSIZE];
	strcpy(testMessage, "LIST");
	testMessage[4] = 0;
	testMessage[5] = 6;
	// ^ DONT FORGET TO SPECIFY THE CORRECT LENGTH FIELD OR YOU WILL GET A SEG FAULT 

	char* ptr = testMessage + 6;
	strcpy(ptr, "Name1:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Name2:22221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Name3:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228Name4:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222229Name7:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222221Na100:12345611111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226");
	//strcpy(ptr, "Name1:12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226Nam77:89221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226");

	open_database("compare.dat");
	printf("past this 2\n");
	handleDiff(testMessage);
	printf("ended method\n");
	close_database();

}*/