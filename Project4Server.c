// Kyong Lee

#include "NetworkHeader.h"
//Add this one to the Network Header eventually
#include <pthread.h>

static char* databaseName;

void *ThreadMain(void *arg);  //Main method of a thread


//Structure of arguments to pass to a client thread
struct ThreadArgs {
  int clntSock; //Socket descriptor for a client
};



int main (int argc, char *argv[])
{
  if (argc != 5) // Test for correct number of arguments
  {
		printf("Error: Usage Project1Server -p <port> -d <database name>\n");
    exit(1);
  } 

  //Changed this from char* to handle the potential problems putting in 
  //the struct, 
  databaseName = argv[4]; // Database
	//printf("database input: %s\n", databaseName); // debugging
  in_port_t servPort = atoi(argv[2]); // Local port

  // Create socket for incoming connections
  int servSock; // Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  // Construct local address structure
  struct sockaddr_in servAddr; // Local address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET; // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
  servAddr.sin_port = htons(servPort); // Local port 

  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithError("bind() failed");

  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, LISTENQ) < 0)
    DieWithError("listen() failed");

  for (;;)
	{
    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithError("accept() failed");

    // clntSock is connected to a client

    //Create seperate memory for client argument
    struct ThreadArgs *threadArgs = (struct ThreadArgs *) = malloc(sizeof(struct ThreadArgs));
    if(threadArgs == NULL) 
      DieWithError("malloc() failed");
    threadArgs->clntSock = clntSock;
    threadArgs->databaseName = databaseName;

    //Create client thread
    pthread_t threadID;
      int returnValue = pthread_create(&threadID, NULL, ThreadMain, threadArgs);
    if(returnValue != 0)
        DieWithError("pthread_create failed");

    //HandleProj4Client(clntSock, databaseName);

  }

  return 0;
}


//Deallocates the thread after completion.
void *ThreadMain(void *threadArgs) {
    //Guarantees that thread resources are deallocated upon return
    pthread_detach(pthread_self());

    //Extract socket file descriptor from argument
    int clntSock = ((struct ThreadArgs *)threadArgs) -> clntSock;
    char* databaseName = ((struct ThreadArgs *)threadArgs) ->databaseName;
    free(threadArgs); //Deallocate argument memory

    //Call method to handle client
    HandleProj4Client(clntSock, databaseName);

    return(NULL);
  }