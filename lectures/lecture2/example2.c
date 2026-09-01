/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only

Always look at man(ual) pages for functions
- man printf
- man 3 printf <- gives library functions
- man 2 printf >- gives system calls*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
   int x = 5; /* x is statically allocated (on the stack)*/
              /*(4 bytes are allocated on teh stack)*/

   printf("x is %d\n", x);
   printf("sizeof( x ) is %lu bytes\n", sizeof(x));
   printf("sizeof( int ) is %lu bytes\n", sizeof( int ));

   int * q = NULL; /* q is statically allocated (on the stack)*/
                  /*(8 bytes are allocated on the stack)*/
#if 0 /* A way to comment out code*/
      printf("*q is %d\n", *q)   /* Cannot dereference a Null pointer*/
#endif
   printf("sizeof( x ) is %lu bytes\n", sizeof(q) );
   printf("sizeof( int ) is %lu bytes\n", sizeof( int ));

   printf("sizeof( x ) is %lu bytes\n", sizeof(*q) );
   printf("sizeof( int ) is %lu bytes\n", sizeof( int ));

   q = &x; /* & is the address -of processor*/

   printf("x is %d\n", x);
   printf("*q is %d\n", *q);

   *q = 13;
   
   printf("x is %d\n",x);
   printf("*q is %d\n", *q);

   char * c = NULL;

   printf("sizeof( c ) is %lu bytes\n", sizeof(c) );
   printf("sizeof( char ) is %lu bytes\n", sizeof( char * ));

   printf("sizeof( x ) is %lu bytes\n", sizeof(q) );
   printf("sizeof( int ) is %lu bytes\n", sizeof( int ));




}



