#include <stdio.h>

/**
Takes as a parameter a pointer to the first of 2 bytes specifying length.
**/
int getLength(char* field)
{
	int byte1 = field[0];
	int byte2 = field[1];

	byte1 = byte1 << 8;

	return byte1 + byte2;

}
/* testing code
main() 
{
	char field[2];
	field[0] = 1;
	field[1] = 5;

	int a = getLength(field);
	printf("%d\n", a);
}*/