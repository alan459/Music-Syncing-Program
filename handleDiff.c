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
void handleDiff(char* receivedList, unsigned long length, char* songs_Not_On_Client, char* songs_Not_On_Server)
{

	char* songs_Not_On_Client = compareSongsToClient(receivedList, length);
	//printf("SONGS NOT ON CLIENT:\n%s \n", songs_Not_On_Client);

	char* songs_Not_On_Server = compareSongsToServer(receivedList, length);
	//printf("SONGS NOT ON SERVER:\n%s \n", songs_Not_On_Server);

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