
#include "WhoHeader.h"
#include "NetworkHeader.h"

/* Classwide constants */
#define MAXIMUM_DATABASE_ENTRY_LENGTH 158
#define SONG_LENGTH 255
#define SHA_LENGTH 128
#define MAX_NUM_RECORDS 500

/* Classwide variables */
static FILE* filePointer;
static char* songList[MAX_NUM_RECORDS]; // list of song:sha pairings in local database
static int* numEntries; // number of entries in local database
static char** nullPtr = NULL;


/* Function Declerations */

void open_database(char* filename);
void close_database();

/* Returns an array of strings where each string represents a song name and sha value of a file 
*  in the local database in the format song_name:sha_value. */
char** lookup_songs (int* no_of_entries);

/* Takes filename and SHA value as parameters and determines if the database contains that file
*  and returns 1 if true and 0 if false. */
int containsSong(char* comparedSha);

/* Given an SHA returns a song name based on SHA to song name mappings in the local database. */
char* getSongName(char* SHA);

/* Takes song name and SHA as a parameter.
*  Adds a song and SHA to the end of the file. 
*  (Assumes file ends with new-line character) */
void addSong(char* songName, char* SHA);

/* Takes as a parameter the list of songs:SHA pairings to compare to our local database contents.
*  Returns a list of songs:SHA pairings of files not stored on our local database based on SHA values. */
char* compareSongs(char* inputBuffer);

/* End Function Declerations */



/****************************************************************************************************************
* Opens the local database where song:SHA pairings are stored and stores them in the classwide array.
* Parameter is the filename of the local database. If the filename is not found an error message is displayed
* and the program exits execution.
****************************************************************************************************************/
void open_database(char* filename) 
{
  if ( (filePointer = fopen(filename,"r+")) == NULL ) {
    fprintf(stderr, "Error: Can't open file %s\n", filename);
    exit(1);
  }

  lookup_songs(&numEntries);
}


/****************************************************************************************************************
* Closes the local database where song:SHA pairings are stored and frees associated memory and variables. 
****************************************************************************************************************/
void close_database() 
{
  fclose(filePointer);
  for (int i = 0; i < numEntries; i++)
  {
    free(songList[i]);
  }
  numEntries = 0;
}


/****************************************************************************************************************
* Takes as a parameter a location in which to store the number of files in the database.
*
* Returns an array containing the song names in the database and their corresponding SHAs
****************************************************************************************************************/
char** lookup_songs (int* no_of_entries) 
{
  char* currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  // for holding song name when going through the database
  char* firstField = malloc(SONG_LENGTH + 1);

  // for holding a song's SHA when going through the database
  char* secondField = malloc(SHA_LENGTH + 1);

  // intialize the number of entries
  *no_of_entries = 0;

  // loop through the entries on each line of the database until null is encountered
  while (fgets(currentLine, MAXIMUM_DATABASE_ENTRY_LENGTH, filePointer) !=  NULL ) 
  { 
      // retrieve the name of the song at the current line
      firstField = strtok(currentLine, ":");

      // retrieve the SHA of the song at the current line and null terminate
      secondField = strtok(NULL, ":");
      secondField[SHA_LENGTH] = '\0';

      // allocate space for a song and SHA entry in the classwide variable
      songList[*no_of_entries] = (char *) malloc(SONG_LENGTH + SHA_LENGTH + 2); 

      // add a song and its SHA to the list of entries in the classwide variable
      sprintf(songList[(*no_of_entries)++],"%s:%s", firstField, secondField);

  }

  // return the location to where the contents of the local database where stored
  return songList;
}




/****************************************************************************************************************
* Takes filename and SHA value as parameters and determines if the database contains that file based on SHA value
* and returns 1 if true and 0 if false.
****************************************************************************************************************/
int containsSong(char* comparedSha) 
{
  // for holding the current song:SHA pairing to be broken apart by strtok()
  char* currentLine = (char *)  malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  // for holding the current SHA retrieved by breaking apart the song:SHA pairing with strtok()
  char* shaField = malloc(SHA_LENGTH + 1);

  // loop through the list of song:SHA pairings in the local database
  for (int i = 0; i < numEntries; i++) 
  {
    // get current song and sha into a temporary variable
    strcpy(currentLine, songList[i]);

    // break the current line along the ':' to prepare to get the SHA
    strtok(currentLine, ":"); 

    shaField = strtok(0, ":"); // get the second field of the current line

    shaField[SHA_LENGTH] = '\0'; // turn new line character to null 

    if(strcmp(comparedSha, shaField) == 0) // if passed in SHA equals current SHA
    {
      return 1; // return true
    }
  }

  return 0; // return false
}


/****************************************************************************************************************
* Given an SHA returns a song name based on SHA to song name mappings in the local database. 
****************************************************************************************************************/
char* getSongName(char* SHA)
{
  char* currentLine = (char *) malloc(MAXIMUM_DATABASE_ENTRY_LENGTH + 1);

  char* nameField = malloc(SONG_LENGTH);

  char* shaField = malloc(SHA_LENGTH + 1);

  // loop through the list of song:SHA pairings in the local database
  for (int i = 0; i < numEntries; i++) 
  {
    // get current song and sha into a temporary variable
    strcpy(currentLine, songList[i]);

    // break the current line along the ':' to prepare to get the SHA
    nameField = strtok(currentLine, ":"); 

    // get the second field of the current line
    shaField = strtok(NULL, ":"); 

    if(strcmp(SHA, shaField) == 0) // if passed in SHA equals current SHA
    {
      return nameField; // return the name of the song
    }
    
  }


  return "";
}


/****************************************************************************************************************
* Takes song name and SHA as a paramete. Adds the song and SHA to the local database.
* 
* Note: Assumes a new line is at the end of the file so if a non-newline character exists at the end it will 
* result in a combined string at the place of the addition.
****************************************************************************************************************/
void addSong(char* songName, char* SHA) 
{

  FILE* append = filePointer;
  fseek(append, 0, SEEK_END); // get top the end of the file

  fprintf(append, "%s:%s\n", songName, SHA); // add to end of file
  fclose(append);
}


/**************************************************************************************************************** 
* Takes as a parameter the list of songs:SHA pairings to compare to our local database contents.
*
* Returns a list of songs:SHA pairings of files not stored on our local database based on SHA values. 
****************************************************************************************************************/
char* compareSongs(char* inputBuffer) 
{
  // allocate a variable for names in the buffer
  char* name = malloc(SONG_LENGTH);
  char* sha = malloc(SHA_LENGTH + 1);

  // string to be returned containing songs
  char* result = malloc(BUFFSIZE);

  char* currentLine = malloc(SONG_LENGTH+ SHA_LENGTH + 1);

  // go through the entire input buffer
  for (int i = 0; i < numEntries; i++)
  {
      // retrieve the current song and sha
      strcpy(currentLine, inputBuffer[i]);

      // get the name of the song only
      name = strtok(currentLine, ":");

      // retrieve the SHA of the song
      sha = strtok(NULL, ":");

      // if the song is not found in the database, add it to output buffer
      if(containsSong(sha) == 0) 
      {
        // add song name : SHA to result to be returned
        strcat(result, name); 
        strcat(result, ":");
        strcat(result, sha);
      }
  }

  // null terminate the result
  strcat(result, "\0");

  return result;
}







