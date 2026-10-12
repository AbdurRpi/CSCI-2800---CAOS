/* fifo-reader.c */

/* A named pipe (a.k.a. fifo) is a unidirectional communication channel */

/* A named pipe does not require a parent/child process relationship */

/* man 3 mkfifo; man 7 pipe */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  char * fifo = "/tmp/fifo1234";   /* named pipe */
    /* this file will ALWAYS be a zero-byte file... */

  /* connect up with the fifo as a reader... */
  int fd = open( fifo, O_RDONLY );
  if ( fd == -1 ) { perror( "open() failed" ); return EXIT_FAILURE; }

  printf( "READER: Opened fifo %s on fd %d for reading\n", fifo, fd );

  /* fd table:
   *
   *  0: stdin
   *  1: stdout
   *  2: stderr                +--------+ think of this buffer as a
   *  3: fd <=====READ=========| buffer |  temporary hidden chunk
   *                           | buffer |   of memory managed by the OS...
   *                           +--------+    (...not part of any
   *                                              process memory space)
   */

  /* read data from the fifo */
  char * buffer = calloc( 32, sizeof( char ) );
  if ( buffer == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }

  while ( 1 )
  {
    int bytes_read = read( fd, buffer, 31 );
    if ( bytes_read == -1 ) { perror( "read() failed" ); return EXIT_FAILURE; }

    if ( bytes_read == 0 )
    {
      printf( "READER: read() returned 0; no data; writer process closed its fd\n" );
                                    /* there are zero write fds open on this fifo */
      break;
    }

    printf( "READER: Read %d bytes: \"%s\"\n", bytes_read, buffer );
  }

  free( buffer );
  close( fd );

  return EXIT_SUCCESS;
}
