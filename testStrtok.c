<<<<<<< HEAD
/* Hello World program */

=======
>>>>>>> a816be1fa69f7f3768e33143c64ab9b7ac475461
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // For isspace
<<<<<<< HEAD

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
=======
// 

main()
{
	char* a = "ddd:ok";
	char* b = malloc(199);
	b = strtok(a, ":");
    printf("Hello %s\n", b);

}
>>>>>>> a816be1fa69f7f3768e33143c64ab9b7ac475461
