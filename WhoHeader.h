#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  // For isspace

void made(int index);

/* Function Declerations */

void open_database(char* filename);
void close_database();
void intializeSongList();

/* Returns an array of strings where each string represents a song name and sha value of a file 
*  in the local database in the format song_name:sha_value. */
char** lookup_songs (int* no_of_entries);

/* Takes filename and SHA value as parameters and determines if the database contains that file
*  and returns 1 if true and 0 if false. */
int containsSong(char* comparedSha);

/* Takes filename and SHA value as parameters and determines if the database contains that file
*  and returns 1 if true and 0 if false. */
int listContainsSong(char* comparedSha, char** inputBuffer, int numBufferEntries);

/* Given an SHA returns a song name based on SHA to song name mappings in the local database. */
char* getSongName(char* SHA);

/* Takes song name and SHA as a parameter.
*  Adds a song and SHA to the end of the file. 
*  (Assumes file ends with new-line character) */
void addSong(char* songName, char* SHA);

/* Takes as a parameter the list of songs:SHA pairings to compare to our local database contents.
*  Returns a list of songs:SHA pairings of files not stored on our local database based on SHA values. */
char* compareSongsToClient(char** inputBuffer, int numBufferEntries);

/* Takes as a parameter the list of songs:SHA pairings to compare to our local database contents.
*  Returns a list of songs:SHA pairings of files not stored on the server database based on SHA values. */
char* compareSongsToServer(char** inputBuffer, int numBufferEntries);

/* Takes in song name find out in database contains song of the same name. */
int fileExists(char* fileName);


/* End Function Declerations */
