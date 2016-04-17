#include "NetworkHeader.h"

const char* byte_to_binary(uint8_t x, char* binary)
{
    binary[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(binary, ((x & z) == z) ? "1" : "0");
    }

    return binary;
}
