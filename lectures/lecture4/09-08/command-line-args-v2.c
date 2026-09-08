/* command-line-args-v2.c */

#include <stdio.h>
#include <stdlib.h>

                 /* char * argv[] */
int main( int argc, char ** argv )
{
  printf( "argc is %d\n", argc );   /* argument count */

  if ( argc != 4 )
  {
    fprintf( stderr, "ERROR: Invalid arguments\n" );
    fprintf( stderr, "USAGE: a.out <filename> <x> <y>\n" );
    return EXIT_FAILURE;
  }

  printf( "argv[0] is %s\n", argv[0] );   /* argv + 0 */
  printf( "argv[1] is %s\n", argv[1] );   /* argv + 1 */
  printf( "argv[2] is %s\n", argv[2] );   /* argv + 2 */
  printf( "argv[3] is %s\n", argv[3] );   /* argv + 3 */
  printf( "argv[argc] is %s\n", argv[argc] );   /* always NULL */

  /* write a loop to print out all of the command-line arguments... */
  for ( int i = 0 ; i < argc ; i++ )
  {
    printf( "argv[%d] is %s\n", i, argv[i] );   /* argv + i */
  }

  /* in C, let's get rid of the use of argc... */
  for ( int i = 0 ; *(argv + i) != NULL ; i++ )
  {
    printf( "argv[%d] is %s\n", i, argv[i] );   /* argv + i */
  }

  /* in C, let's get rid of the use of argc... */
  for ( int i = 0 ; *(argv + i) ; i++ )  /* no need for != NULL in condition */
  {
    printf( "argv[%d] is %s\n", i, argv[i] );   /* argv + i */
  }

  /* rewrite this loop without using argc and without using int variable... */
  for ( char ** ptr = argv ; *ptr ; ptr++ )
  {
    printf( "next argument is %s\n", *ptr );
    /* TO DO: use pointer arithmetic and some math to determine */
    /*         which index we are at, i.e., 0, 1, 2, ...        */
  }

  /*  ptr++  ==>  ptr = ptr + 1  ==>  ptr = ptr + 1 x sizeof( char * )  */

  return EXIT_SUCCESS;
}

#if 0
                 char *
                +------+
  argv ---> [0] |  =======> "./a.out"
                +------+
            [1] |  =======> "a.txt"
                +------+
            [2] |  =======> "12"
                +------+
            [3] |  =======> "34"
                +------+
            [4] | NULL |
                +------+
#endif
