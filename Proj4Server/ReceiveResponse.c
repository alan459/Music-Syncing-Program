#include "NetworkHeader.h"

// given a packet, looks at the length field (4-5th bytes) and returns
// its corresponding decimal value as unsigned long
unsigned long retrieveLength(char* packet);

// Receives and sets response packet to response.
// Returns length field of response pakcet as unsigned long.
unsigned long receiveResponse(int sock, char* response)
{
	// receive response message from server
	int retrievedLength = 0; // boolean representing whether we have retrieved value from length field
	unsigned long length_Message = 0; // 2 byte field contains length of message (does not count type field)
	int totalBytesRcvd = 0; // total number of bytes received
	for (;;)
	{
		char buffer[BUFFSIZE];
		ssize_t numBytesRcvd = recv(sock, buffer, BUFFSIZE-1, 0);
		//printf("numBytesRcvd: %zu\n", numBytesRcvd); // debugging
		//printf("hello buffer received: %s\n", buffer); // debugging
		if (numBytesRcvd < 0)
			DieWithError("recv() failed");
		else if (numBytesRcvd == 0)
			DieWithError("recv() failed: connection closed prematurely");
		buffer[numBytesRcvd] = '\0'; // append null-character

		// append received buffer to response
		int u;
		for (u = totalBytesRcvd; u < totalBytesRcvd+numBytesRcvd; u++)
		{
			response[u] = buffer[u-totalBytesRcvd];
		}

		// retrieve the length field from message. (located 4th-5th bytes)
		if (!retrievedLength && numBytesRcvd >= 6)
		{
			length_Message = retrieveLength(response);

			retrievedLength = 1;
		}

		// update totalBytesRcvd;
		totalBytesRcvd = totalBytesRcvd + numBytesRcvd;

		printf("totalBytesRcvd: %i\nlength_Message: %lu\n", totalBytesRcvd, length_Message); // DEBUGGING
		printf("packet in receiveResponse: ");
		for (u = 0; u < totalBytesRcvd; u++)
		{
			printf("%c", response[u]);
		}
		printf("\n");

		// if message received is length_Message long, exit the loop
		// 4 for type field, 2 for length field
		if (totalBytesRcvd == 4 + 2 + length_Message)
		{
			printf("ENTERED TOTAL!!!!\n");
			response[totalBytesRcvd] = '\0';
			return length_Message;
		}

	}
}
