/* simple.c*/
/* compile via:
Bash$ gcc -Wall -Werror simple.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 

Always look at man(ual) pages for functions
- man printf
- man 3 printf <- gives library functions
- man 2 printf >- gives system calls*/

#include <stdio.h>
#include <stdlib.h> /* #define EXIT_SUCCESS*/
#include <unistd.h>
#include <math.h> /* gcc ... -lm <== links the math library */

int main(){
   int x;

   for ( x = 1 ; x < 8; x++){
      printf ("%3d %f %20.15f\n", x, sqrt(x), sqrt(x));
   }

   /* Read THrough the prinf f mans pages for different outputs*/


   return EXIT_SUCCESS; /* 0 ==> bash$ echo $?*/
}