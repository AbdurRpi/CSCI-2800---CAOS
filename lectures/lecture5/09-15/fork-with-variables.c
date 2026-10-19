/* fork-with-variables.c */

/* fork() is a system call that creates a new (child) process */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  /* ALL static and dynamic memory allocations are DUPLICATED */
  /*  in the child process when we call fork()...             */
  int x = 5;
  char * name = malloc( 32 );
  strcpy( name, "DAVID" );
     /* TO DO: add code that changes and displays name... */

  pid_t p;  /* pid_t is essentially unsigned int */

  p = fork();  /* attempt to create a new (child) process, */
               /*  which is a duplicate of this parent process */
               /*             ^^^^^^^^^                        */

  if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }

  /* we now have TWO separate running a.out processes... */

  if ( p == 0 )     /* CHILD process runs this code... */
  {
    x += 100;
    printf( "CHILD: Happy birthday to me! x is %d\n", x );
    printf( "CHILD: My process id (PID) is %d\n", getpid() );
  }
  else /* p > 0 */  /* PARENT process runs this code... */
  {
    x += 15;
    printf( "PARENT: My new child process has PID %d; x is %d\n", p, x );
    printf( "PARENT: My process id (PID) is %d\n", getpid() );
  }

  /* TO DO: run this code using valgrind --- you should see a memory leak
   *         in the parent process and a memory leak in the child process!
   */

#if 0
  /* BOTH parent and child processes will continue running code here... */
  free( name );
#endif

  return EXIT_SUCCESS;
}
