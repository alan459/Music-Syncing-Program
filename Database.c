
#include "WhoHeader.h"

#define MAXIMUM_DATABASE_ENTRY_LENGTH 158
#define SONG_LENGTH 30
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


/**
* Takes as a parameter a location in which to store the number of files in the database.
* Returns an array containing the song names in the database and their corresponding SHAs
**/
char** lookup_songs (int* no_of_entries) 
{

  char* currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  // for holding song name when going through the database
  char* firstField = malloc(SONG_LENGTH + 1);

  // for holding song SHA when going through the database
  char* secondField = malloc(SHA_LENGTH + 1);

  // intialize the number of entries
  *no_of_entries = 0;

  // loop through the entries on each line of the database until null is encountered
  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) !=  NULL ) 
  { 
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

    shaField = strtok(NULL, ":")); // get the second field of the current line

    if(strcmp(comparedSha, shaField) == 0) // if passed in SHA equals current SHA
    {
      return 1; // return true
    }

  }

  return 0; // return false
}


/** 
* Given an SHA returns a song name. 
**/
char* getSongName(char* SHA)
{
  char* currentLine = (char *)  malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  char* shaField = malloc(SHA_LENGTH + 1);

  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) != NULL )
  {
    shaField = strtok(NULL, ":"));
    if(strcmp(comparedSha, shaField)==0) {
      return 1;
    }
  }

  return 0;

  return "";
}


/**
* Takes song name and SHA as a parameter.
* Adds a song and SHA to the end of the file.
* 
* Note: Assumes a new line is at the end of the file so 
* if a non-newline character exists at the end it will result 
* in a combined string.
**/
void addSong(char* songName, char* SHA) 
{

  FILE* append = filePointer;
  fseek(append, 0, SEEK_END); // get top the end of the file

  fprintf(append, "%s:%s\n", songName, SHA); // add to end of file
  fclose(append);
}



/**
* Takes as a parameter the list of songs/SHAs to compare to our local database contents.
*
* Returns a list of songs/SHAs of files not stored on our local database.
**/
char* compareSongs(char* inputBuffer) 
{
  // allocate a variable for names in the buffer
  char name[SONG_LENGTH];

  //allocat a variable for sha in the buffer
  char sha[SHA_LENGTH];

  // generically allocated output buffer
  char result[BUFFSIZE];

  // move input buffer pointer past message type to start of first song
  inputBuffer += 4;

  // go through the entire input buffer
  while(inputBuffer[0] == '\0') 
  {
    // Retrieve next Name 1 character at a time
    strncpy(name, inputBuffer, SONG_LENGTH);

    // move input buffer pointer to start of next SHA
    inputBuffer += SONG_LENGTH;

    //Retrieve next SHA
    strncpy(sha, inputBuffer, SHA_LENGTH);

    // move input buffer pointer past SHA to start of next song
    inputBuffer += SHA_LENGTH;

    // if the song is not found in the database, add it to output buffer
    if(containsSong(sha) == 0) 
    {
      strcat(result,name);
      strcat(result,":");
      strcat(result,sha);
      strcat(result,"\n");
    }
  }

  //null terminate the buffer
  strcat(result, "\0");

}






