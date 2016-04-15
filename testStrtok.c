/* Hello World program */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // For isspace

//


main()
{
	char* a = malloc(189);
	strcpy(a, "ddd:ok");
	char* b = malloc(199);
	strtok(a, ":");
	b = strtok(0, ":");
    printf("Hello %s\n", b);

}
