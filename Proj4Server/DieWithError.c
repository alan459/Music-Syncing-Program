// Kyong Lee

#include <stdio.h>
#include <stdlib.h>

/*
 * Print the error message to stderr and exit the program.
 */
void DieWithError(const char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
