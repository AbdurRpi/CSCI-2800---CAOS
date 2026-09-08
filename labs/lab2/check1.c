/* Check 1*/
/*
Bash$ gcc -Wall -Werror check1.c
Bash$ gcc -Wall -Werror check2.c -lm
Bash$ gcc -E -Wall -Werror check1.c <== preprocessor only


*/

#include <stdio.h>
#include <stdlib.h>

int main(){
   /* The 0x prefix indicates a hexdecimal (base 16) number*/
   #if 0
   int secret = 0x534f4143; the big end is 53 and the little end is 43 the most significant is always on the left 
   beacuse it refers to how much that byte contibutes to the numerical value.
   #endif
   int secret = 0x59454552;
   //int secret = 0x07;

   char * c = (char *)&secret; /* type casting...*/

   printf( "%c",*c++);
   printf( "%c",*c++);
   printf( "%c",*c++);
   printf( "%c\n",*c++);

   return EXIT_SUCCESS;
/* First run came back with CAOS little endianness*/
}