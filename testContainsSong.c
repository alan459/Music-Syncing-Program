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
    shaField[strlen(shaField) - 1] = '\0';
    printf("sha field %s\n", shaField);
    printf("length is %d\n", strlen(shaField));
    if(strcmp(comparedSha, shaField) == 0) // if passed in SHA equals current SHA
    {
      return 1; // return true
    }

  }

  return 0; // return false
}




main()
{
	open_database("database.dat");
	int d  = containsSong("12221111111111111222222222222222222221222111111111111122222222222222222222N12221111111111111222222222222222222222222222222222226");
    printf("Hello %d\n", d);

}
