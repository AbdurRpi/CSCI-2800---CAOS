/* pipe.c */

/* A "pipe" is a unidirectional communication channel -- man 2 pipe */
/*               ^^^^^^^^^^^^^^                          man 7 pipe */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int pipefd[2];  /* array to hold the two pipe (file) descriptors:
                   *  pipefd[0] is the "read" end of the pipe
                   *  pipefd[1] is the "write" end of the pipe
                   */

  /* create the pipe... */
  int rc = pipe( pipefd );
  if ( rc == -1 ) { perror( "pipe() failed" ); return EXIT_FAILURE; }

  /* fd table:
   *
   *  0: stdin
   *  1: stdout
   *  2: stderr                +--------+ think of this buffer as a
   *  3: pipefd[0] <===READ====| buffer |  temporary hidden chunk
   *  4: pipefd[1] ====WRITE==>| buffer |   of memory managed by the OS...
   *                           +--------+    (...not part of any
   *                                              process memory space)
   */

  printf( "Created pipe: pipefd[0] is %d; pipefd[1] is %d\n",
          pipefd[0], pipefd[1] );

  /* write data to the pipe */
  int bytes_written = write( pipefd[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26 );
  printf( "Wrote %d bytes to the pipe\n", bytes_written );

  /* read data from the pipe */
  char * tmp = calloc( 32, sizeof( char ) );
  if ( tmp == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }

  int bytes_read = read( pipefd[0], tmp, 10 );
  /* TO DO: check the return value for -1 here... */
  printf( "First read: %d bytes: \"%s\"\n", bytes_read, tmp );
    /* NOTE that when we read from a pipe, the data read is REMOVED
     *  from the pipe
     */

  bytes_read = read( pipefd[0], tmp, 10 );
  printf( "Second read: %d bytes: \"%s\"\n", bytes_read, tmp );

  bytes_read = read( pipefd[0], tmp, 10 );
  *( tmp + bytes_read ) = '\0';   /* assume this is character data */
    /* TO DO: add a '\0' character after EACH read() call above... */
  printf( "Third read: %d bytes: \"%s\"\n", bytes_read, tmp );

#if 1
  /* read() is a BLOCKING call for pipes...
   *  ... because there is at least one open write descriptor
   *       on the pipe
   */
  bytes_read = read( pipefd[0], tmp, 10 );  /* <== BLOCKED here */
  *( tmp + bytes_read ) = '\0';   /* assume this is character data */
  printf( "Fourth read: %d bytes: \"%s\"\n", bytes_read, tmp );
#else
  close( pipefd[1] );  /* close the WRITE end of the pipe */

  /* read() is a BLOCKING call for pipes...
   *  ... unless there are zero open write descriptors
   *       on the pipe --- in that case, read() returns 0 immediately
   */
  bytes_read = read( pipefd[0], tmp, 10 );  /* <== return 0 */
  *( tmp + bytes_read ) = '\0';   /* assume this is character data */
  printf( "Fourth read: %d bytes: \"%s\"\n", bytes_read, tmp );
#endif

  close( pipefd[0] );  /* close the READ end of the pipe */

  free( tmp );

  return EXIT_SUCCESS;
}








