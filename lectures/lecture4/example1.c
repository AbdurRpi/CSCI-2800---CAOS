/* dynamic-mem-two-layer-structure.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
#if 0
  int * x;  /* array of int */
#endif

  char ** names;   /* array of char* ... */

  names = calloc( 64, sizeof( char * ) );    /* top layer */
  if ( names == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }

  names[2] = malloc( 7 );   /* malloc( 7 * sizeof( char ) ); */
  if ( names[2] == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }

  strcpy( names[2], "LAKERS" );
  strcpy( *(names + 2), "LAKERS" );

  printf( "Let's go, %s!\n", names[2] );
  printf( "Let's go, %s!\n", *(names + 2) );

  /* resize names[2] to be smaller (7 down to 6) */
  *(names + 2) = realloc( *(names + 2), 6 );
  printf( "Let's go, %s!\n", *(names + 2) );   /* bug (valgrind) */
  sprintf( *(names + 2), "76ERS" );
  printf( "Let's go, %s! (just kidding)\n", *(names + 2) );

#if 1
  free( *(names + 2) );   /* place this in a loop for 0..63 */
#endif
  free( names );

  return EXIT_SUCCESS;

  /* QUIZ question pointer arithmetic and how to naviagte through the array below */
}

#if 0
  BEFORE:       char *       512 bytes allocated for names array
               +------+      (64*8)
  names --> [0]| NULL |
               +------+
            [1]| NULL |     [0] [1] [2] [3] [4] [5] [6]
               +------+    +---+---+---+---+---+---+----+
            [2]|  =======> | L | A | K | E | R | S | \0 |
               +------+    +---+---+---+---+---+---+----+
                 ...
               +------+
           [63]| NULL |
               +------+


  AFTER:        char *       512 bytes allocated for names array
               +------+      (64*8)
  names --> [0]| NULL |
               +------+
            [1]| NULL |     [0] [1] [2] [3] [4] [5]
               +------+    +---+---+---+---+---+----+
            [2]|  =======> | 7 | 6 | E | R | S | \0 |
               +------+    +---+---+---+---+---+----+
                 ...
               +------+
           [63]| NULL |
               +------+
#endif
