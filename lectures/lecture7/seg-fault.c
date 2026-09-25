/* seg-fault.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
#if 0
  main();   /* seg-fault due to unbounded recursion */
#endif

  int * y = calloc( 1234, sizeof( int ) );

  *(y + 100) = 5555;
  printf( "%d %d\n", *(y + 100), *(y + 101) );

  *(y + 1000000) = 6789;  /* seg-fault! */

  free( y );

  return EXIT_SUCCESS;
}
