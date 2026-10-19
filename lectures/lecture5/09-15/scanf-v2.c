/* scanf-v2.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{                       /* a pointer is a memory address... */
  int x;
  printf( "Enter value: " );
  scanf( "%d", &x );    /* <== & is the address-of operator */
  printf( "Thanks, x is %d\n", x );

  char name[8];
  printf( "Enter your name: " );
  scanf( "%s", name );               /* what happens (in memory) when you */
  printf( "Hi, %s\n", name );        /*  input data larger than 8 bytes?  */

  return EXIT_SUCCESS;
}
