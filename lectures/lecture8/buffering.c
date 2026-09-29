/* buffering.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* When printing to the terminal/shell via stdout (fd 1),
 *  output is buffered in memory by printf()...
 *
 * The newline '\n' character will empty ("flush") the
 *  stdout buffer, i.e., output everything stored in the
 *   buffer so far...
 *
 * ^^^^ this is line-based buffering; default behavior for fd 1 with printf()
 *
 * When we instead redirect stdout (fd 1) to a file...
 *
 *  bash$ ./a.out > output.txt
 *
 *  ...the '\n' character no longer flushes the stdout buffer!
 *
 *   the output.txt file is empty and the in-memory stdout printf() buffer:
 *    "HERE0HERE1\nHERE2HERE2.5\nABCD"
 *
 * ^^^^ this is block-based buffering or fully buffered mode
 *
 * A third type of buffering is non-buffered (or unbuffered),
 *  which is what is used for stderr (fd 2)
 */

int main()
{
#if 1
  /* read the man page for setvbuf() */
  setvbuf( stdout, NULL, _IONBF, 0 );    /* <== change stdout to be unbuffered */
#endif

printf( "HERE0" );                           /* stdout buffer: "HERE0" */
  int * y = calloc( 1234, sizeof( int ) );
printf( "HERE1\n" );                         /* stdout buffer: "HERE0HERE1\n" */
                                             /*  (flushed to stdout)          */
  *(y + 100) = 5555;
printf( "HERE2" );                           /* stdout buffer: "HERE2" */
printf( "HERE2.5\nABCD" );                   /* stdout buffer: "HERE2HERE2.5\nABCD" */
                                             /*  (flushed up to the '\n' to stdout) */
                                             /* stdout buffer: "ABCD" */
#if 0
fflush( NULL ); /* flush ALL buffers */
#endif

  *(y + 1000000) = 6789;  /* seg-fault! */
printf( "HERE3" );
  free( y );
printf( "HERE4" );
  return EXIT_SUCCESS;
printf( "HERE5" );
}
