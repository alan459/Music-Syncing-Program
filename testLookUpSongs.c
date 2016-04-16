
#include "WhoHeader.h"

#define MAXIMUM_DATABASE_ENTRY_LENGTH 158
#define FIRSTFIELD_LENGTH 30
#define SECONDFIELD_LENGTH 128
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
* Takes as a parameter database name?
* Returns an array containing the song names in the database and their corresponding SHAs
* Might
**/
char** lookup_songs (int* no_of_entries) 
{
  char* currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  char* firstField = malloc(FIRSTFIELD_LENGTH + 1);
  char* secondField = malloc(SECONDFIELD_LENGTH + 1);

  *no_of_entries = 0;

  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) !=  NULL ) 
  {
      firstField = strtok(currentLine, ":");
      //secondField = strtok(NULL, ":")) == NULL;

      if (( secondField = strtok(NULL, ":")) == NULL) {
         break; 
      }


      songList[*no_of_entries] = (char *) malloc(200);
      sprintf(songList[(*no_of_entries)++],"%s:%s", firstField, secondField);

  }

  return songList;
}


int main() {

  open_database("database.dat");
  int nums = 0;
  char** a = lookup_songs(&nums);
  for(int i = 0; i < nums; i++) {
     printf("%s\n", a[i]);
  }
 

  return 0;
}








