/* Static allocations
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/
/* man strlen */

/* Re-read pointers arrays, static and dynamic memory with examples*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

   char * string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
         /* string is a static allocation (8 bytes)*/
   char name[5] = "DAVID";
         /*       [0] [1] [2] [3] [4] [5]*/
         /* name: | D | A | V | I | D |*/
         /* name is a static allocation(5 bytes)*/
   printf("Hello, %s\n", name);
   name[1] = 'a'; /* side note: tolower in the ctype.h lb*/
   *(name+1) = 'a';
   printf("%s\n", string);
   printf("Hello again, %s\n", name);
   printf("Length of name is %ld\n", strlen(name));

   char name2[5] = "Annie";
   /* TO DO: move this up yo just beneath name[] declaration */
   printf("And hello, %s\n", name2);

   /* The bug is that strings in c must end in "\o" character*/
   return EXIT_SUCCESS;
}