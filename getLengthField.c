#include <stdio.h>

/**
* Takes as a parameter a pointer to the first of 2 bytes of a field specifying length.
*
* Shifts the first byte 8 orders of magnitude to the left and adds it to the value
* of the bit on the right
**/
unsigned int getLength(char* field)
{
	return (unsigned int) ( (field[0] << 8) + field[1] );

}
/* testing code
main() 
{
	char field[2];
	field[0] = 127;
	field[1] = 5;

	unsigned int a = getLength(field);
	printf("%u\n", getLength(field));
}*/