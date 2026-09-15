/* compile via:
Bash$ gcc -Wall -Werror check2.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

unsigned int fibn(unsigned int n){
   if (n <= 1){
      return n;
   }
   return fibn(n -1) + fibn(n - 2);
      }

unsigned long fibl(unsigned int n){
   if (n <= 1){
      return n;
   }
   return fibl(n - 1) + fibl(n - 2);
}

int main(void){
   int i;
   //int INT;
   //long LONG;

   printf("This program calculates fib(n).");

while(1){
   printf("Enter n (or -1 to exit): \n");
   
   if(scanf("%d", &i) != 1){
      perror("Error: invalid input\n");
      return EXIT_FAILURE;
      //printf("%d\n", n);
   }
   if ( i == -1){
      break;
   }
   if ( i < 0){
      continue;
   }
   printf("Using unsigned int, fib(%d) is %u.\n", i, fibn((unsigned int )i));
   printf("Using unsigned long, fib(%d) is %lu.\n", i, fibl((unsigned int )i));

}
return EXIT_SUCCESS;
}
/*
What is the range of valid values for unsigned int and unsigned long?

for unsigned int valid range is 0 - 4,294,967,295 of 32 bit size

for unsigned long valid range is 0 - 18,446,744,073,709,551,615 of 64 bit size

Why does it take such a long time to compute larger values?

It takes longer because each Fibonacci call creates two more 
recursive calls and repeatedly recalculates the same earlier 
values. As the input grows, the number of calls increases 
exponentially, requiring dramatically more computation.

*/


