// Kyong Lee

#include "NetworkHeader.h"

/* function declarations */

const char* byte_to_binary(uint8_t x, char* binary);

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
			
			// receive listResponse message from server
			char listResponse[BUFFSIZE]; // Buffer for received message
			listResponse[0] = '\0'; // initialize to empty C-string
			int retrievedLength = 0; // boolean representing whether we have retrieved value from length field
			unsigned long length_Message = 0; // 2 byte field contains length of message (does not count type field)
			int totalBytesRcvd = 0; // total number of bytes received
			for (;;)
			{
				//printf("Entered infinite loop\n"); // debugging
				char buffer[BUFFSIZE];
				ssize_t numBytesRcvd = recv(sock, buffer, BUFFSIZE-1, 0);
				//printf("numBytesRcvd: %zu\n", numBytesRcvd); // debugging
				//printf("hello buffer received: %s\n", buffer); // debugging
				if (numBytesRcvd < 0)
					DieWithError("recv() failed");
				else if (numBytesRcvd == 0)
					DieWithError("recv() failed: connection closed prematurely");
				buffer[numBytesRcvd] = '\0'; // append null-character
				
				// append received buffer to listResponse
				int u;
				for (u = totalBytesRcvd; u < totalBytesRcvd+numBytesRcvd; u++)
				{
					listResponse[u] = buffer[u-totalBytesRcvd];
				}

				// retrieve the length field from message. (located 4th-5th bytes)
				if (!retrievedLength && numBytesRcvd >= 6)
				{
					char firstBin[9]; char secondBin[9];
					byte_to_binary(listResponse[4], firstBin);
					byte_to_binary(listResponse[5], secondBin);
					strcat(firstBin, secondBin);
					length_Message = (unsigned long)strtoul(firstBin, NULL, 2);
					//printf("length_Message: %lu\n", length_Message); // DEBUGGING

					retrievedLength = 1;
				}

				// update totalBytesRcvd;
				totalBytesRcvd = totalBytesRcvd + numBytesRcvd;

				// if message received is length_Message long, exit the loop
				// 4 for type field, 2 for length field
				if (totalBytesRcvd == 4 + 2 + length_Message)
				{
					//printf("totalBytesRcvd: %i\n length_Message: %lu\n", totalBytesRcvd, length_Message); // DEBUGGING
					break;
				}

			}

/*
			// print listResponse DEBUGGING
			int j;
			for (j = 0; j < totalBytesRcvd; j++)
			{
				printf("%c", listResponse[j]);
			}
			printf("\n"); */
			
			// print the names of the songs in the server to stdout
			printf("Song name \t SHA\n");
			int i;
			for (i = 0; i < length_Message/(MAX_SONGNAME_LENGTH+SHA_LENGTH); i++)
			{
				// retrieve song name
				char currentSongName[MAX_SONGNAME_LENGTH+1];
				strncpy(currentSongName, listResponse+4+2+i*(MAX_SONGNAME_LENGTH+SHA_LENGTH), MAX_SONGNAME_LENGTH);
				
				// retrieve SHA DEBUGGING
				char currentSHA[SHA_LENGTH+1];
				strncpy(currentSHA, listResponse+4+2+i*(MAX_SONGNAME_LENGTH+SHA_LENGTH)+MAX_SONGNAME_LENGTH, SHA_LENGTH);

				// print song name and SHA
				printf("%s \t %s\n", currentSongName, currentSHA);
			}

		}
break; // DEBUGGING
	}
	

	// construct PULL message
	char pullMessage[BUFFSIZE];
	strcpy(pullMessage, PULLType);
	// message length is SHA_LENGTH
	pullMessage[5] = SHA_LENGTH;
	pullMessage[4] = 0x0;
	strcat(pullMessage, "12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226"); // debugging
	

	//printf("hello-message sent: %s", helloMessage); // debugging
	//printf("helloLength: %d\n", helloLength); // debugging

	// send the PULL message to the server
	ssize_t numBytes = send(sock, pullMessage, 4+2+SHA_LENGTH, 0);
	//printf("numBytes sent for PULL: %zu\n", numBytes); // debugging
	if (numBytes < 0)
		DieWithError("send() failed");
	else if (numBytes != 4+2+SHA_LENGTH)
		DieWithError("send() failed: sent unexpected number of bytes");

	return 0;
}
/*
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
} */
