/* simple.c*/
/* compile via:
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only

Always look at man(ual) pages for functions
- man printf
- man 3 printf <- gives library functions
- man 2 printf >- gives system calls*/

#include <stdio.h>
#include <stdlib.h> /* #define EXIT_SUCCESS*/
#include <unistd.h>
#include <math.h> /* gcc ... -lm <== links the math library */
#define LOOP_COUNT 8

int main(){
   int x;

   for ( x = 1 ; x < 8; x++){
      printf ("%3d %f %20.15f\n", x, sqrt(x), sqrt(x));
   }

   /* Read THrough the prinf f mans pages for different outputs*/


   return EXIT_SUCCESS; /* 0 ==> bash$ echo $?*/
}

/* Runtime Heap : Is a dynamic allocation (at Runtime) in (C++/java)
Runtime Heap Static allocation (Compile Time)
Each activation record tracks each function call (at runtime)
and defines 

Scanf(&x). &x < address of operator

Each functions call in memory stacking from main is called an
- Activation record



*/