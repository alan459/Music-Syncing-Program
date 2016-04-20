// Kyong Lee

#include "NetworkHeader.h"

/* function declarations */

void getSong(char* songName, char* song, int* numBytes);
void storeSong(char* songName, char* song, int numBytes);

// given a packet, looks at the length field (4-5th bytes) and returns
// its corresponding decimal value as unsigned long
unsigned long retrieveLength(char* packet);

// prints every song and SHA combination from listResponse.
// numEntries represents number of song and SHA combinations in listResponse.
void printLIST(char* listResponse, unsigned long numEntries);

// Constructs and sends LIST message to server.
void sendLIST(int sock);

// Receives and sets response packet to response.
// Returns length field of response pakcet as unsigned long.
unsigned long receiveResponse(int sock, char* response);


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

	// ask user for command (list, diff, sync, leave)
	printf("Please Enter Command in Small Case:\n");
	char* command = (char*) malloc(5); 
	scanf("%s", command);
	
	while (strcmp(command, "leave") != 0)
	{
		if (strcmp(command, "list") == 0)
		{

			// send LIST message to server
			sendLIST(sock);

			// receive listResponse from server
			char listResponse[BUFFSIZE];
			unsigned long length_Message = receiveResponse(sock, listResponse);

			/*
			// print listResponse DEBUGGING
			int j;
			for (j = 0; j < totalBytesRcvd; j++)
			{
				printf("%c", listResponse[j]);
			}
			printf("\n"); */
			
			// print every song and SHA combination from listResponse
			printLIST(listResponse, length_Message);

			scanf("%s", command); // read another command from user
		}

		else if (strcmp(command, "pull") == 0)
		{
			// construct PULL message
			char pullMessage[BUFFSIZE];
			strcpy(pullMessage, PULLType);

			// message length is SHA_LENGTH
			pullMessage[5] = (uint16_t)SHA_LENGTH;
			pullMessage[4] = (uint16_t)SHA_LENGTH >> 8;


						unsigned long lengthMessage = retrieveLength(pullMessage);
						printf("lengthMessage: %lu\n", lengthMessage); // DEBUGGING

			// DEBUGGING
			char tmpSHA[128] = "00000000011111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228";
			int k;
			for (k = 0; k < 128; k++)
			{
				pullMessage[6+k] = tmpSHA[k];
			}
			// DEBUGGING print pullmessage that is about to be sent to server
			int j;
			for (j = 0; j < 134; j++)
			{
				printf("%c", pullMessage[j]);
			}
			printf("\n");

			// send the PULL message to the server
			ssize_t numBytes = send(sock, pullMessage, 4+2+SHA_LENGTH, 0);
			//printf("numBytes sent for PULL: %zu\n", numBytes); // debugging
			if (numBytes < 0)
				DieWithError("send() failed");
			else if (numBytes != 4+2+SHA_LENGTH)
				DieWithError("send() failed: sent unexpected number of bytes");

			// receive pullResponse message from server
			char pullResponse[BUFFSIZE]; // create pullResponse
			receiveResponse(sock, pullResponse);

			// retrieve song name
			char songName[MAX_SONGNAME_LENGTH+1];
			strncpy(songName, pullResponse+4+2, MAX_SONGNAME_LENGTH);

			// retrieve song file
			char song[MAX_SONG_LENGTH+1];
			strncpy(song, pullResponse+4+2+MAX_SONGNAME_LENGTH, MAX_SONG_LENGTH);

			// print song name and song file DEBUGGING
			printf("%s \t %s\n", songName, song);

		}
		
		else if (strcmp(command, "push") == 0)
		{
			// construct PUSH message
			char pushMessage[BUFFSIZE];
			strcpy(pushMessage, PUSHType);

			// append message length
			unsigned long messageLen = MAX_SONGNAME_LENGTH + SHA_LENGTH + 15;
			printf("ORIGINAL messageLen: %lu\n", messageLen);
			pushMessage[5] = (uint16_t)messageLen;
			pushMessage[4] = (uint16_t)messageLen >> 8;
			
						unsigned long lengthMessage = retrieveLength(pushMessage);
						printf("lengthMessage: %lu\n", lengthMessage); // DEBUGGING

			char songName[255] = "Name2";
			// append null characters for the rest of songName
			int r;
			for (r = MAX_SONGNAME_LENGTH; r >= 5; r--)
			{
				songName[r] = '\0';
			}

			int i;
			for (i = 0; i < 255; i++)
			{
				pushMessage[6+i] = songName[i];
			}

			char sha[128] = "00000000011111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222228";
			for (i = 0; i < 128; i++)
			{
				pushMessage[6+255+i] = sha[i];
			}

			char file[15] = "0124810354 6242";
			for (i = 0; i < 128; i++)
			{
				pushMessage[6+255+128+i] = file[i];
			}

			// print pushMessage
			for (i = 0; i < 6+255+128+15; i++)
			{
				printf("%c", pushMessage[i]);
			}
			printf("\n");

			// send pushMessage to server
			ssize_t numBytes = send(sock, pushMessage, 4+2+MAX_SONGNAME_LENGTH+SHA_LENGTH+15, 0); // 15 MUST BE REPLACED BY FILE LENGTH LATER
			if (numBytes < 0)
				DieWithError("send() failed");
			else if (numBytes != 4+2+MAX_SONGNAME_LENGTH+SHA_LENGTH+15) // 15 MUST BE REPLACED BY FILE LENGTH LATER
				DieWithError("send() failed: sent unexpected number of bytes");
			
			break;
		}

		else if (strcmp(command, "sync") == 0)
		{/*
			// send LIST message to server
			sendList(sock);

			// receive listResponse from server
			char listResponse[BUFFSIZE];
			unsigned long length_Message = receiveResponse(sock, listResponse);

			// calculate different song files between client and server
			char* clientSongs // songs that are in client but not in server
			char* serverSongs // songs that are in server but not in client
			handleDiff(listResponse+6, length_Message, clientSongs, serverSongs);

			// retrieve lengths of clientSongs and serverSongs
			unsigned long lengthClient = retrieveLength(clientSongs);
			unsigned long lengthServer = retrieveLength(serverSongs);
			printf("lengthMessage: %lu\n", lengthClient); // DEBUGGING
			printf("lengthMessage: %lu\n", lengthServer); // DEBUGGING

			// push all the songs to server from clientSongs
			int i;
			for (i = 0; i < lengthClient; i++)
			{
				// retrieve song file from database
				
			}*/
		}

	}

	return 0;
}

// Constructs and sends LIST message to server.
void sendLIST(int sock)
{
	// construct LIST message
	char listMessage[BUFFSIZE];
	strcat(listMessage, LISTType);
	// length field is zero
	listMessage[4] = 0x0;
	listMessage[5] = 0x0;
	
	// send LIST message to server
	ssize_t numBytesSent = send(sock, listMessage, 4+2, 0);
	if (numBytesSent < 0)
	{
		DieWithError("send() failed");
	}
}


// prints every song and SHA combination from listResponse.
// numEntries represents number of song and SHA combinations in listResponse.
void printLIST(char* listResponse, unsigned long numEntries)
{
	// print the names of the songs in the server to stdout
	printf("Song name \t SHA\n");
	int i;
	for (i = 0; i < numEntries/(MAX_SONGNAME_LENGTH+SHA_LENGTH); i++)
	{
		// retrieve song name
		char currentSongName[MAX_SONGNAME_LENGTH+1];
		strncpy(currentSongName, listResponse+4+2+i*(MAX_SONGNAME_LENGTH+SHA_LENGTH), MAX_SONGNAME_LENGTH);
		currentSongName[MAX_SONGNAME_LENGTH] = '\0';
	
		// retrieve SHA DEBUGGING
		char currentSHA[SHA_LENGTH+1];
		strncpy(currentSHA, listResponse+4+2+i*(MAX_SONGNAME_LENGTH+SHA_LENGTH)+MAX_SONGNAME_LENGTH, SHA_LENGTH);
		currentSHA[SHA_LENGTH] = '\0';

		// print song name and SHA
		printf("%s \t %s\n", currentSongName, currentSHA);
	}
}

/*
// Constructs and sends PUSH message to server
void sendPUSH(int sock, unsigned long length, char* songName, char* SHA, char* songFile)
{

}*/
