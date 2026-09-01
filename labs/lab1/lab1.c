/* compile via:
Bash$ gcc -Wall -Werror simple.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 

Always look at man(ual) pages for functions
- man printf
- man 3 printf <- gives library functions
- man 2 printf >- gives system calls*/

#include <stdio.h>
#include <math.h>

/* Check Point 2 write a program that displays a table of data showing the natural logrithm and exponential functions with base e of powers 2 ranging from 2^0 to 2^7 */

int x;

int main(){

   double logn, expn;

   printf("                      x");
   printf("  x  ln(x)           e");
   printf("---  --------------  -----------------");
   for (int x = 0; x <= 7; x++)
      expn = exp(x);
      logn = log(expn);
      printf(" %d  %.13f           %.13f");
   printf("---  --------------  -----------------");

   


}