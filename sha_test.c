#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void sha512_hash_string(unsigned char hash[SHA512_DIGEST_LENGTH], char outputBuffer[129]) { 
    int i = 0;
    for(i = 0; i < SHA512_DIGEST_LENGTH; i++) {    
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[128] = 0;
}

void sha512(char *string, char outputBuffer[129]) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    SHA512_Update(&sha512, string, strlen(string));
    SHA512_Final(hash, &sha512);
    int i = 0;
    for(i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[128] = 0;
}

int sha512_file(char *path, char outputBuffer[129]) {
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

int main(int argc, char *argv[]) {
    char buffer[129];
    //sha512("string", buffer);
    sha512_file("/home/at2pj/cs332/Project4/sha_test/song1.mp3", buffer);
    printf("%s\n", buffer);
}
