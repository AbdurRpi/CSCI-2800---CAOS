/* dynamic-mem-two-layer-structure-v3.c */
/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/

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

  strcpy( names[2], "LAKERS" );      /* both of these lines are equivalent */
  strcpy( *(names + 2), "LAKERS" );  /* both move forward 16 bytes...      */

  printf( "Let's go, %s!\n", names[2] );
  printf( "Let's go, %s!\n", *(names + 2) );

  /* resize names[2] to be smaller (7 down to 6) */
  *(names + 2) = realloc( *(names + 2), 6 );
#if 0
  printf( "Let's go, %s!\n", *(names + 2) );   /* bug (valgrind) */
#endif
  sprintf( *(names + 2), "76ERS" );
  printf( "Let's go, %s! (just kidding)\n", *(names + 2) );

  /* resize names[2] to be larger (6 up to 13 */
  *(names + 2) = realloc( *(names + 2), 13 );
  strcpy( *(names + 2), "LEBRON JAMES" );
  printf( "Let's go, %s!\n", *(names + 2) );
  /* Both staments are equivalent V*/
  printf("Fourth Char: %c\n", names[2][3]);
  printf("Fourth Char: %c\n", *(*(names+ 2) + 3));
#if 1
  free( *(names + 2) );   /* place this in a loop for 0..63 */
#endif
  free( names );

  return EXIT_SUCCESS;
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
                 ...                                ^^^^
               +------+                       this byte could be reused
           [63]| NULL |                        in a future calloc()/malloc()
               +------+                         call...


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

  AFTER:        char *       512 bytes allocated for names array
               +------+      (64*8)
  names --> [0]| NULL |
               +------+
            [1]| NULL |     [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10][11][12]
               +------+    +---+---+---+---+---+---+---+---+---+---+---+---+----+
            [2]|  =======> | L | E | B | R | O | N |   | J | A | M | E | S | \0 |
               +------+    +---+---+---+---+---+---+---+---+---+---+---+---+----+
                 ...
               +------+
           [63]| NULL |
               +------+

                             e.g., heap memory:

                                   76ERS\0 ABCDEFGHIJKLMNOPQRSTUVWXYZ
                                  (6bytes) (26bytes.................)

                                        new loc: 76ERS\0........
                                                 (13bytes......)

                            for realloc(), always use this pattern:

                                  x = realloc( x, new_size );

#endif
