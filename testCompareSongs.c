/* Hello World program */
#include "WhoHeader.h"
#include "NetworkHeader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // For isspace

#define MAXIMUM_DATABASE_ENTRY_LENGTH 158
#define SONG_LENGTH 255
#define SHA_LENGTH 128
#define MAX_NUM_RECORDS 100

static FILE* filePointer;
static char* songList[MAX_NUM_RECORDS];
static char** nullPtr = NULL;
static int* numEntries;

void open_database ( char *filename) {
  if ( (filePointer = fopen(filename,"r+")) == NULL ) {
    fprintf(stderr, "Error: Can't open file %s\n", filename);
    exit(1);
  }
}

void close_database() 
{
  fclose(filePointer);
}



/**
* Takes as a parameter a location in which to store the number of files in the database.
*
* Returns an array containing the song names in the database and their corresponding SHAs
**/
char** lookup_songs (int* no_of_entries) 
{
  printf("line 41\n");
  char* currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);
  printf("43\n");
  // for holding song name when going through the database
  char* firstField = malloc(SONG_LENGTH + 1);
printf("46\n");
  // for holding song SHA when going through the database
  char* secondField = malloc(SHA_LENGTH + 1);
printf("49\n");
  // intialize the number of entries
  *no_of_entries = 0;
  printf("line 52\n");
  // loop through the entries on each line of the database until null is encountered
  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) !=  NULL ) 
  { 
      printf("in while loop\n");
      // retrieve the name of the song
      firstField = strtok(currentLine, ":");

      // retrieve the SHA of the song
      if (( secondField = strtok(NULL, ":")) == NULL) {
         break; 
      }

      secondField[SHA_LENGTH] = '\0';

      // allocate space for a song and SHA entry
      songList[*no_of_entries] = (char *) malloc(SONG_LENGTH + SHA_LENGTH + 2); 

      // add a song and its SHA to the list of entries in the array
      sprintf(songList[(*no_of_entries)++],"%s:%s", firstField, secondField);

  }

  return songList;
}




/*
* Takes filename and SHA value as parameters and determines if the database contains that file
* and returns 1 if true and 0 if false.
**/
int containsSong(char* comparedSha) 
{
  char* currentLine = (char *)  malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  char* shaField = malloc(SHA_LENGTH + 1);

  // loop through the lines of the database 
  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) != NULL )
  {
    strtok(currentLine, ":"); // give strtok() a reference to the current line????

    shaField = strtok(NULL, ":"); // get the second field of the current line
    shaField[strlen(shaField) - 1] = '\0'; // turn new line character to null 

    if(strcmp(comparedSha, shaField) == 0) // if passed in SHA equals current SHA
    {
      return 1; // return true
    }

  }

  return 0; // return false
}


/**
* Takes as a parameter the list of songs/SHAs to compare to our local database contents.
*
* Returns a list of songs/SHAs of files not stored on our local database.
**/
char* compareSongs(char** inputBuffer, int numEntries) 
{
  // allocate a variable for names in the buffer
  char* name = malloc(SONG_LENGTH);
  char* sha = malloc(SHA_LENGTH);

  // string to be returned containing songs
  char* result = malloc(BUFFSIZE);

  // move input buffer pointer past message type to start of first song
  //inputBuffer += 4;
  printf("point 1\n");

  // go through the entire input buffer
  for (int i = 0; i < numEntries; i++)
  {

      // retrieve the name of the song
      name = strtok(inputBuffer[i], ":");

      // retrieve the SHA of the song
      sha = strtok(NULL, ":");

      printf("iteration %d\n", i);

    // if the song is not found in the database, add it to output buffer
    if(containsSong(sha) == 0) 
    {
      // add song name : SHA to result to be returned
      strcat(result, name); 
      strcat(result, ":");
      strcat(result, sha);

      strcat(result, "\n");
    }
  }

  // null terminate the buffer
  strcat(result, "\0");

  return result;

}


main()
{
	
  open_database("compare.dat");
  int* num;
  printf("opened\n");
  int* rrr;
  char** songList = lookup_songs(rrr);  
  printf("b4 close\n");
  close_database();
  printf("closed\n");

  open_database("database.dat");
  char* result  = compareSongs(songList, *rrr);

	//char* d  = "ff";
  printf("Hello %s\n", result);

}
