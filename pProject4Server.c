#include "NetworkHeader.h"

char *databaseName = NULL;

void *ThreadMain(void *arg); // Main program of a thread

// Structure of arguments to pass to client thread
struct ThreadArgs 
{
  int clntSock;
};

int main (int argc, char *argv[])
{
  if (argc != 5) // Test for correct number of arguments
  {
    printf("Error: Usage Project4Server -p <port> -d <database name>\n");
    exit(1);
  }

  databaseName = argv[4]; // Database
  //printf("database input: %s\n", databaseName); // DEBUGGING
  in_port_t servPort = atoi(argv[2]); // Local port

  // Create socket for incoming connections
  int servSock; // Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
  {
    DieWithError("socket() failed");
  }

  // Construct local address structure
  struct sockaddr_in servAddr; // Local address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET; // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
  servAddr.sin_port = htons(servPort); // Local port 

  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) 
  {
    DieWithError("bind() failed");
  }

  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, LISTENQ) < 0)
  {
    DieWithError("listen() failed");
  }

  for (;;)
  {
    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0) 
    {
      DieWithError("accept() failed");
    }

    // Print client address
    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
    {
      //printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    } 
    else
    {
      puts("Unable to get client address");
    }

    // Create separate memory for client argument
    struct ThreadArgs *threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
    if (threadArgs == NULL) 
    {
      DieWithError("malloc() failed");
    }
    threadArgs->clntSock = clntSock;

    // Create client thread
    pthread_t threadID;
    int returnValue = pthread_create(&threadID, NULL, ThreadMain, threadArgs);
    if (returnValue != 0) 
    {
      DieWithError("pthread_create() failed");
      printf("with thread %lu\n", (unsigned long int) threadID);
    }
  }
  /* NOT REACHED */
}

void *ThreadMain(void *threadArgs) 
{
  // Guarantees that thread resources are deallocated upon return
  pthread_detach(pthread_self());

  // Extract socket file descriptor from argument
  int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
  
  free(threadArgs); // Deallocate memory for argument

  HandleProj4Client(clntSock, databaseName);

  return NULL; 
}

