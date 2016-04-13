
#include "WhoHeader.h"

#define MAXIMUM_DATABASE_ENTRY_LENGTH 158
#define FIRSTFIELD_LENGTH 30
#define SECONDFIELD_LENGTH 128
#define MAX_NUM_RECORDS 100

static FILE* filePointer;
static char* songList[MAX_NUM_RECORDS];
static char** nullPtr = NULL;
static int* no_of_entries;

void open_database ( char *filename) {
  if ( (filePointer = fopen(filename,"r+")) == NULL ) {
    fprintf(stderr, "Error: Can't open file %s\n", filename);
    exit(1);
  }
}


/**
* Takes as a parameter database name?
* Returns an array containing the song names in the database and their corresponding SHAs
**/
char** lookup_songs (char* filename) {

  char *currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  char* firstField = malloc(FIRSTFIELD_LENGTH + 1);
  char* secondField = malloc(SECONDFIELD_LENGTH + 1);

  no_of_entries = 0;

  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) !=  NULL ) 
  {
      firstField = strtok(currentLine, ":");
      //secondField = strtok(NULL, ":")) == NULL;

      if (( secondField = strtok(NULL, ":")) == NULL) {
         break;
      }


      songList[no_of_entries] = (char *) malloc(100);
      sprintf(songList[(no_of_entries)++],"%s:%s", firstField, secondField);

  }

  return songList;
}


/*
* This method determines if a given sha value is contained in the
* database for the client/server, and returns the appropriate boolean.
**/
bool containsSong(char* filename, char* comparedSha) {
  char *currentLine = (char *)  malloc(MAXIMUM_DATABASE_ENTRY_LENGTH+1);

  char* shaField = malloc(SECONDFIELD_LENGTH + 1);

  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) != NULL)
  {
    shaField = strtok(NULL, ":"));
    if(strcmp(comparedSha, shaField)==0) {
      return true;
    }
  }
  return false;
}


/**
* Takes song name and SHA as a parameter.
* Adds a song and SHA to the end of the file.
* 
* Note: Assumes a new line is at the end of the file so 
* if a non-newline character exists at the end it will result 
* in a combined string.
**/
void addSong(char* songName, char* SHA, char* filename) 
{

  FILE* append = filePointer;
  fseek(append, 0, SEEK_END); // get top the end of the file

  fprintf(append, "%s:%s\n", songName, SHA); // add to end of file
  fclose(append);
}







