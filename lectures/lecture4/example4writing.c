/* fd-read.c*/

/* A "file" is simply a stream of bytes -- read and/or write*/
/* file descirptor (fd) --  a small non-negative integer used in a variety of system calls
      to refer to an open file (ie. file stream or byet stream)
      
         ( a "process" is a program in exicution...)
      - each proccess has a file descirptor table associated with it that keeps track of 
         its open (file) descriptors
      
         fd.      c++  Java        C
         0 stdin  cin  System.in.  scanf(), fgetc(), read(),...
         1 stdout cout System.out  printf, write(), ...
         2 stderr cerr System.err  perror("calloc() failed");
                                    fprintf( stderr, "Error:...\n");
         stdout and stderr both display on the terminal (in the shell)
      */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
   char * name = "testfile.txt";
      /* attempt to open the testfile.txt file
         O_WRONLY for writing
         O_CREAT create the file, if necessary
         O_TRUNC
                     */

   int fd = open( name, O_WRONLY | O_CREAT | O_TRUNC | 0640 );
                                                     /* ^^*/
                            /* Leading 0 mean this is octal (base 8)^^*/
                            /* Get the LECTURE NOTES TO FILL IN*/
   if ( fd == -1){ perror( "open()failed");}
   /* Read man page for open() -- man open()*/

   /* fd table fro this running process:
   * 0 stdin
   * 1 stdout
   * 2 stderr
   * 3 testfile.txt (O_RDONLY)
   */
  /* read from the file */
  char buffer[32]; /* In memory buffer overwrites itself and does not allocate more memory*/
  int rc = read( fd, buffer, 16);
  buffer[rc] = '\0';  /* Always explicatly add a backslash 0 (\0)*/
  printf( " first read() returned %d -- buffer \"%s\n", rc, buffer);
   rc = read( fd, buffer, 16);
  buffer[rc] = '\0';  /* Always explicatly add a backslash 0 (\0)*/
  printf( " second read() returned %d -- buffer \"%s\n", rc, buffer);
   rc = read( fd, buffer, 16);
  buffer[rc] = '\0';  /* Always explicatly add a backslash 0 (\0)*/
  printf( " third read() returned %d -- buffer \"%s\n", rc, buffer);

  while (1){
   int rc = read( fd, buffer, 16); /* Check for error here ... */
   if ( rc == 0) break;
   buffer[rc] = '\0';
   printf( "subsequent read() returned %d -- buffer \"%s\n", rc, buffer);
#if 0
   sleep( 3 ); /* delay (suspend the process for 3 seconds...) */
#endif
  }


  close( fd );


  /* QUIZ QUESTION  
   what is the error in the program
   no NULL terminating character ("\0") explicitly defined*/

   return EXIT_SUCCESS;
}