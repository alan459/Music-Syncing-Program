/* Hello World program */

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


/** 
* Given an SHA returns a song name. 
* Returns a song name corresponding to a passed in SHA or an empty string if song not found.
**/
char* getSongName(char* SHA)
{
  char* currentLine = (char *)  malloc(MAXIMUM_DATABASE_ENTRY_LENGTH+1);

  char* nameField = malloc(SONG_LENGTH + 1);

  char* shaField = malloc(SHA_LENGTH + 1);

  while ( fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) != NULL )
  {
    nameField = strtok(currentLine, ":");

    shaField = strtok(NULL, ":");
    shaField[strlen(shaField) - 1] = '\0'; // turn new line character to null 

    if(strcmp(SHA, shaField) == 0) 
    {
      return nameField;
    }
  }

  return "";
}



main()
{
	open_database("database.dat");
	char* d  = getSongName("12322");
    printf("Hello %s\n", d);

}
