/* fifo-writer.c */

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

  /* create the fifo in the writer process... */
  int rc = mkfifo( fifo, 0660 );   /* see also fd-write.c */
  if ( rc == -1 ) { perror( "mkfifo() failed" ); return EXIT_FAILURE; }

  printf( "WRITER: Created fifo with handle %s\n", fifo );



  /* connect up with the fifo as a writer... */
  int fd = open( fifo, O_WRONLY );
  if ( fd == -1 ) { perror( "open() failed" ); return EXIT_FAILURE; }

  printf( "WRITER: Opened fifo %s on fd %d for writing\n", fifo, fd );

  /* fd table:
   *
   *  0: stdin
   *  1: stdout
   *  2: stderr                +--------+ think of this buffer as a
   *  3: fd =======WRITE======>| buffer |  temporary hidden chunk
   *                           | buffer |   of memory managed by the OS...
   *                           +--------+    (...not part of any
   *                                              process memory space)
   */

  /* write data to the fifo */
  int bytes_written = write( fd, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26 );
  if ( bytes_written == -1 ) { perror( "write() failed" ); return EXIT_FAILURE; }

  printf( "WRITER: Wrote %d bytes\n", bytes_written );

  /* TO DO: add more write() calls here... */

  close( fd );


#if 1
  /* marking the fifo for deletion... see: man 2 unlink */
  printf( "WRITER: Marking the fifo for deletion...\n" );

  rc = unlink( fifo );
  if ( rc == -1 ) { perror( "unlink() failed" ); return EXIT_FAILURE; }
#endif

  return EXIT_SUCCESS;
}




