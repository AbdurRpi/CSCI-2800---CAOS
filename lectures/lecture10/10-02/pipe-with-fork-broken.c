/* pipe-with-fork-broken.c */

/* A "pipe" is a unidirectional communication channel -- man 2 pipe */
/*               ^^^^^^^^^^^^^^                          man 7 pipe */

/* This version is "broken" since both parent and child
 *  processes have BOTH the read and write descriptors to
 *   to the pipe open....
 */

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

  /* fork() duplicates the fd table to the child process */
  pid_t p = fork();
  if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }
  
  /* fd table:
   *
   *  <<PARENT>>                                      <<CHILD>>
   *  0: stdin                                        0: stdin
   *  1: stdout                                       1: stdout 
   *  2: stderr                +--------+             2: stderr
   *  3: pipefd[0] <===READ====| buffer |====READ===> 3: pipefd[0]
   *  4: pipefd[1] ====WRITE==>| buffer |<===WRITE=== 4: pipefd[1]
   *                           +--------+
   */

  if ( p == 0 )  /* CHILD */
  {
#if 0
    sleep( 5 );
#endif
    /* write data to the pipe */
    int bytes_written = write( pipefd[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26 );
    printf( "CHILD: Wrote %d bytes to the pipe\n", bytes_written );

#if 0
    /* read data from the pipe */
    char * tmp = calloc( 32, sizeof( char ) );
    if ( tmp == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }
    int bytes_read = read( pipefd[0], tmp, 10 );
    printf( "CHILD: read: %d bytes: \"%s\"\n", bytes_read, tmp );
    free( tmp );
#endif
  }
  else  /* PARENT */
  {
    /* read data from the pipe */
    char * tmp = calloc( 32, sizeof( char ) );
    if ( tmp == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }

    int bytes_read = read( pipefd[0], tmp, 10 );
    printf( "PARENT: First read: %d bytes: \"%s\"\n", bytes_read, tmp );

    bytes_read = read( pipefd[0], tmp, 10 );
    printf( "PARENT: Second read: %d bytes: \"%s\"\n", bytes_read, tmp );

    bytes_read = read( pipefd[0], tmp, 10 );
    *( tmp + bytes_read ) = '\0';   /* assume this is character data */
    printf( "PARENT: Third read: %d bytes: \"%s\"\n", bytes_read, tmp );

    free( tmp );
  }

  return EXIT_SUCCESS;
}








