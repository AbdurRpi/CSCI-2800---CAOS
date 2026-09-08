/*
char = 1 byte
c - 1 byte
short - 2 bytes
int - 4 bytes
long - 8 bytes
float - 4 bytes
double - 8 bytes

pointer values are always the size of the memory adress
so in this case all values are of size 8 bits (on 64-bit architecture)


add unassigned to any of these data types
Each integer data type has a valid range of values



Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only

Always look at man(ual) pages for functions
- man printf
- man 3 printf <- gives library functions
- man 2 printf >- gives system calls
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
   /* int *x: array of int*/

   char ** names; /* array of char* ...*/
   names = calloc(64, sizeof( char*)); /* top layer*/ 
   /* The entire preogram creates a 2 layer array (NOT 2 dimensional) see Diagram*/
   if ( names == NULL) {perror("calloc() failed"); return EXIT_FAILURE;}

   names[2] = malloc(7); /* malloc(7* sizeof( char ));  - Same equivalent*/
   if ( names == NULL) {perror("calloc() failed"); return EXIT_FAILURE;}
   strcpy(names[2], "LAKERS");
   strcpy(*(names + 2), "LAKERS");

   printf( "lets go, %s!\n", names[2]);
   /*  You will need to use pointer arithmatic in this variation with no square brackets like ^^^^*/
   printf( "lets go, %s!\n", *(names + 2));
   /* resize names[2] to be smaller from ((7 in malloc) down to 5)*/
   *(names + 2) = realloc( *(names + 2), 5);
   printf( "lets go, %s!\n", *(names + 2)); /* Bug Valgrind*/
   sprintf( *(names + 2), "76ERS");
   printf( "Lets go, %s! (just kidding)\n", *(names +2));

   /* READ THROUGH MAN PAGES OF calloc malloc realloc*/
   /* Explain the dereferencing and intilizaling*/
   free( *(names + 2)); /* place this in a looop for 0...63 to free up allocated memory space*/
   free(names);

   

   return EXIT_SUCCESS;
}

/* QUIZ QUESTIONS:

How many bytes are allocated for names array ? 
ANSWER 512 - beacuse its 64 * 8 = 512 (64 bit architecture and pointer byte value )*/