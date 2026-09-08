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
   #if 0
   int x[1000];
   #endif
   /* Dynamically allocate array of char of size 100 bytes*/
   char * s = malloc( 100 );
   if (s == NULL) {perror("malloc() failed"); return EXIT_FAILURE;}
   printf("s: \"%s\"\n", s);

   s[0] = 'A';
   s[1] = 'B';
   s[2] = 'C';
   /* s[3] = '\0' */

   s[20] = 'x';
   s[21] = 'y';
   s[22] = 'z';
   s[23] = '\0';

   printf("s: \"%s\n",s);
   printf("s[20]: \"%s\n", &s[20] ); /* xyz*/
   printf("s+20: \"%s\n", s + 20 );  /* xyz*/
   /* If you only wanted 1 character */

   printf("s+20: '%c'\n", *(s +20)); /* X */

   free(s);
   /* Use valgrind here in dynamic memory 
   allocation to check for silent errors and 
   memory dynamic memory allocation issues*/
   /* we are accessing bytes read from the 100 byte array from the printf statement*/
   printf("s: \"%s\"\n", s);
   free(s);
#if 0
DIfference between malloc and calloc
Malloc:  will not initialze the data or each indivisual elements to 0

Calloc: It initializes each elelemnt to 0 first

Memset() : loops through evey byte of memory and intilizes values to 0 

   int * x = malloc(100 * sizeof( int ));
   if (x == NULL) {perror("malloc() failed"); return EXIT_FAILURE;}
   int * x = calloc(100, sizeof(int));
   if (x == NULL) {perror("malloc() failed"); return EXIT_FAILURE;}
#endif
int * x = calloc(100, sizeof(int));
   if (x == NULL) {perror("malloc() failed"); return EXIT_FAILURE;}
x[32] = 1234;
printf("x[32]: %d\n", x[32]);
printf("*(x +32): &d\n", *(x + 32));

free( x );

   return EXIT_SUCCESS;
}