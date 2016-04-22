#include "WhoHeader.h"
#include "NetworkHeader.h"

void sha512_hash_string(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[SHA_LENGTH+1]);
int sha512_file(char *path, char outputBuffer[SHA_LENGTH+1]);
void updateFilesInDatabase();

/**************************************************************************************************************
* Takes in a new file and adds its name to the database containing the list of file names/sha
**************************************************************************************************************/
void updateFilesInDatabase() 
{
    // open up the current file directory
    DIR *fileDir;
    fileDir = opendir(".");
    // temp structure for directory entry
    struct dirent *dirEntry;
    // open up our name database, statically called database.dat
    FILE* nameBase = fopen("database.dat", "w");
    // make a file path to fetch file for sha computation
    char shaBuffer[SHA_LENGTH+1];

    if (nameBase == NULL) 
    {
        printf("%s\n", "Error opening name database.");
        exit(1);
    }

    if (fileDir) 
    {
        while ((dirEntry = readdir(fileDir)) != NULL) 
        {
            // get file extension
            char *ext = strrchr(dirEntry->d_name, '.');
            if (!ext) {
                continue;
            }
            // printf("%s\n", ext); //DEBUG
            // only add files to database if it is an .mp3 file
            if (!strcmp(ext, ".mp3"))
            {
                //printf("%s\n", dirEntry->d_name); //DEBUG
                sha512_file(dirEntry->d_name, shaBuffer);
                // write it to the name:sha database
                fprintf(nameBase, "%s:%s\n", dirEntry->d_name, shaBuffer);
            }
        }
    }

    // close the name:sha database
    fclose(nameBase);
}

// Compute SHA512 for strings
void sha512_hash_string(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[SHA_LENGTH+1]) 
{ 
    int i = 0;
    for(i = 0; i < SHA512_DIGEST_LENGTH; i++) {    
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[SHA_LENGTH] = 0;
}

// Compute SHA512 for files
int sha512_file(char *path, char outputBuffer[SHA_LENGTH+1]) 
{
    FILE *file = fopen(path, "rb");
    if(!file) return -1;

    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    const int bufSize = 32768;
    char  *buffer = malloc(bufSize);
    int bytesRead = 0;
    if(!buffer) return -1;
    while((bytesRead = fread(buffer, 1, bufSize, file))) {
        SHA512_Update(&sha512, buffer, bytesRead);
    }
    SHA512_Final(hash, &sha512);

    sha512_hash_string(hash, outputBuffer);
    fclose(file);
    free(buffer);
    return 0;
}

