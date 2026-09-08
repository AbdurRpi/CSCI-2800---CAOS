/* fd-write.c */

/**
 * A "file" is simply a stream of bytes --- read and/or write
 *
 * file descriptor (fd)
 *
 * -- a small non-negative integer used in a variety of system calls
 *     to refer to an open file (i.e., file stream or byte stream)
 *
 *      (a "process" is a program in execution...)
 * -- each process has a file descriptor table associated with it
 *     that keeps track of its open (file) descriptors
 *
 * fd         C++   Java        C
 *  0 stdin   cin   System.in   scanf(), fgetc(), read(), ...
 *  1 stdout  cout  System.out  printf(), write(), ...
 *  2 stderr  cerr  System.err  perror( "calloc() failed" );
 *                              fprintf( stderr, "ERROR: ...\n" );
 *
 * stdout and stderr both display on the terminal (in the shell)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  char * name = "testfile.txt";

                    /* attempt to open the testfile.txt file...
                     * O_WRONLY for writing
                     * O_CREAT create the file, if necessary
                     * O_TRUNC truncate the file if it already exists
                     *         (set file size to zero)
                     * (check out O_APPEND)
                     */
  int fd = open( name, O_WRONLY | O_CREAT | O_TRUNC, 0640 );
                    /*                               ^^^^
                     *                                |
                     *               leading 0 means this is octal (base 8)
                     *
                     * if we do create the file...
                     *  0640 ==> 110 100 000 (binary)
                     *           rwx rwx rwx
                     *           ^^^ ^^^ ^^^
                     *            |   |   |
                     *            |   |  no permissions for other/world
                     *            |   |
                     *            |  r for group permissions
                     *            |
                     *           rw for the file owner
                     */

  if ( fd == -1 ) { perror( "open() failed" ); return EXIT_FAILURE; }

  printf( "fd is %d\n", fd );

  /* fd table for this running process:
   *
   *  0 stdin
   *  1 stdout
   *  2 stderr
   *  3 testfile.txt (O_WRONLY)
   */

  /* write to the file */
  int rc = write( fd, "SPARKS", 6 );  /* TO DO: change 6 to different values */
  printf( "Wrote %d bytes to fd %d\n", rc, fd );

  /* Note that the '\0' character is NOT written to the file...
   *
   * We need the '\0' character in memory to indicate the end of the string,
   *  but we do NOT need the '\0' character written to the file...
   */

  int important = 32768;
  rc = write( fd, &important, sizeof( int ) );
  printf( "Wrote %d bytes to fd %d\n", rc, fd );

  /* HINT: use hexdump to display the bytes (in hexadecimal!) of the file...
   *
   *       bash$ hexdump -C testfile.txt
   */

  close( fd );

  return EXIT_SUCCESS;
}



