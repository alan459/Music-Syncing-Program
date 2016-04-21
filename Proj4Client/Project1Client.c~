// Alan Malayev
// Jan 25 2016
// CS 332
// FINAL CLIENT CODE FOR PROJ2

#include "NetworkHeader.h"


/* function declarations */

void HandleProj1Client(int cliSock, char *randString, char *serverHost, unsigned short serverPort);

// Sets up the connection and returns the socket associated with that connection
int SetupClientSocket(const char *host, const char *service);

// Returns the first index after 2 words in a string
// returns -1 if failed
int skipNWords(char* cookieString, int n);

// Returns the second field in the string
char* getCookie(char cookieString[BUFFSIZE]);

/* end of function declarations */



int main (int argc, char *argv[]) {

  /* There are certainly slicker ways of doing this,
   * but I figured there's no need to make the code
   * any more confusing than it has to be at this point.
   */

  char *firstName;
  char *lastName;
  char *serverHost = SERVER_HOST;
  unsigned short serverPort = atoi(SERVER_PORT);
  char *servPortString;
  char c;
  int i;

  if ((argc < 5) || (argc > 7)) {
    printf("Error: Usage Project0Client [-s <server IP>[:<port>]] -f <firstName> -l <lastName>\n");
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
           serverPort = atoi(servPortString);
          }
         break;

        default:
         break;
      }
    }
  }

  /* Your networking code starts here */

  // create string to be sent
  char echoString[BUFFSIZE];
  strcpy( echoString, "LIST\n");

  // set up connection on a socket
  int sock = SetupClientSocket(serverHost, servPortString);
  if (sock < 0)
    DieWithError("socket() failed"); 

  HandleProj1Client(sock, echoString, serverHost, serverPort);

            

  close(sock);
  exit(0); 

}


// Sets up the connection and returns the socket associated with that connection
int SetupClientSocket(const char *host, const char *service) {

 // Tell the system what kind(s) of address info
 struct addrinfo addrCriteria;
 memset(&addrCriteria, 0, sizeof(addrCriteria));
 addrCriteria.ai_family = AF_UNSPEC;
 addrCriteria.ai_socktype = SOCK_STREAM;
 addrCriteria.ai_protocol = IPPROTO_TCP;

 struct addrinfo *servAddr; // Holder for returned list of server addrs
 int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);

 if (rtnVal != 0)
  DieWithError("getaddrinfo() failed");

 int sock = -1;
 struct addrinfo* addr;
 for (addr = servAddr; addr != NULL; addr = addr->ai_next) {
   // Create a reliable, stream socket using TCP
   sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
   if (sock < 0)
    continue; // Socket creation failed; try next address

   // Establish the connection to the echo server
   if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
    break; // Socket connection succeeded; break and return socket 32

   close(sock); // Socket connection failed; try next address
   sock = -1;
 }

 freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo() 38 
 return sock;

 }


void HandleProj1Client(int cliSock, char *randString, char *serverHost, unsigned short serverPort)
{


  // Determine length of the first messsage to be sent
  size_t echoStringLen = strlen(randString); 

  // SEND the FIRST string to the server
  ssize_t numBytes = send(cliSock, randString, echoStringLen, 0);

  // CHECK if the SEND was SUCCESSFUL
  if (numBytes < 0)
    DieWithError("send() failed");
  else if (numBytes != echoStringLen)
    DieWithError("send() - sent unexpected number of bytes");


  // Receive the response message (ack) from the server
  fputs("Received: ", stdout); 
   char buffer[BUFFSIZE];

  // printf("reached handleporj1client\n"); \\ debugging
  // receive message from client
  char rcvMessage[BUFFSIZE]; // Buffer for received message
  rcvMessage[0] = '\0'; // initialize to empty C-string
  for (;;)
  {
    //printf("Entered infinite loop\n"); // debugging
   // char buffer[BUFFSIZE];
    ssize_t numBytesRcvd = recv(cliSock, buffer, BUFFSIZE-1, 0);
    //printf("numBytesRcvd: %zu\n", numBytesRcvd); // debugging
    //printf("hello buffer received: %s\n", buffer); // debugging
    if (numBytesRcvd < 0)
      DieWithError("recv() failed");
    else if (numBytesRcvd == 0)
      DieWithError("recv() failed: connection closed prematurely");
    buffer[numBytesRcvd] = '\0'; // append null-character
    strcat(rcvMessage, buffer); // append the received buffer to ackMessage

    // if new-line character is received, exit the loop
    if (buffer[numBytesRcvd-1] == '\n')
    {
      //printf("new-line char received\n"); // debugging
      break;
    }
    //printf("end\n"); // debugging
  }




  // BYE MESSAGE to be sent back to the server
  char message[BUFFSIZE]; 

  strcpy(message, "CS332 BYE "); 

  // extract the cookie from the string containing the cookie
  char* cook = getCookie(buffer); 
  // assuming the string contains 3 words, with the final being the cookie

  // add the cookie to the message
  strcat(message, cook); 
  strcat(message, "\n");

  // LENGTH of the BYE message to be sent back to the server
  size_t messageLength = strlen(message);

  // SEND the BYE message to the server
  ssize_t numBytes2 = send(cliSock, message, messageLength, 0);

  // CHECK if SEND failed
  if (numBytes2 < 0)
    DieWithError("send() failed");
  else if (numBytes2 != messageLength)
    DieWithError("send() - sent unexpected number of bytes");

  //fputc('\n', stdout); // new line to stdout
}


// Returns the cookie field in the string
char* getCookie(char cookieString[BUFFSIZE])
{
  int i = skipNWords(cookieString, 2);
   
  char* cookie = malloc(BUFFSIZE); 
  /* gets the extracted cookie under the assumption that it is the third field
     in the string. */

  int j;
  for(j = 0; cookieString[i] != '\0' && cookieString[i] != '\n' ; j++)
  {
     cookie[j] = cookieString[i];
     i++;
  }
 // printf("returned cookie: %s\n", cookie);
  return cookie;
}

// Returns the first index after 2 words in a string
// returns -1 if failed
int skipNWords(char* cookieString, int n)  
{
  // to hold the index of first whitespace char after 2nd word
  int i = 0; 

  int word;
  for (word = 1; word <= n; word++) {

    // skip whitespace before word
    for ( ; cookieString[i] == ' ' ; i++ ) 
    {
       if (cookieString[i] == '\0')
        return -1;
    }

    // skip non-whitespace characters
    for ( ; cookieString[i] != ' ' ; i++ ) {  }

   }

  // skip whitepsace immediately before cookie
  for ( ; cookieString[i] == ' ' ; i++ ) { }
  
  // return index where (N+1)th word starts or null occurs
  return i;
   
} 


void DieWithError(char *errorMessage)
{
  perror(errorMessage);
  exit(0);
}
