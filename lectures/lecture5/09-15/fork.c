/* fork.c */

/* fork() is a system call that creates a new (child) process */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  pid_t p;  /* pid_t is essentially unsigned int */

  p = fork();  /* attempt to create a new (child) process, */
               /*  which is a duplicate of this parent process */

  if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }

  /* we now have TWO separate running a.out processes... */

  if ( p == 0 )     /* CHILD process runs this code... */
  {
    printf( "CHILD: Happy birthday to me!\n" );
    printf( "CHILD: My process id (PID) is %d\n", getpid() );
#if 0
    printf( "CHILD: My parent's process id (PID) is %d\n", getppid() );
    /* revisit why this value is sometimes 1 or some other PID value...? */
#endif
  }
  else /* p > 0 */  /* PARENT process runs this code... */
  {
    printf( "PARENT: My new child process has PID %d\n", p );
    printf( "PARENT: My process id (PID) is %d\n", getpid() );
  }

  return EXIT_SUCCESS;
}

#if 0
HOW MANY POSSIBLE OUTPUTS ARE THERE FOR THIS CODE?

                        p = fork()
                         /      \
       p > 0  <parent>  /        \  <child>  p == 0
                       /          \
 =========================      =========================
 PARENT: My new child...        CHILD: Happy birthday...
 PARENT: My process id...       CHILD: My process id...
 =========================      =========================


goldsd3@linux-new:~/f26/csci2800$ gcc -Wall fork.c 
goldsd3@linux-new:~/f26/csci2800$ 
goldsd3@linux-new:~/f26/csci2800$ ./a.out 
CHILD: Happy birthday to me!
PARENT: My new child process has PID 3817228
CHILD: My process id (PID) is 3817228
PARENT: My process id (PID) is 3817227
goldsd3@linux-new:~/f26/csci2800$ ./a.out 
PARENT: My new child process has PID 3817321
PARENT: My process id (PID) is 3817320
CHILD: Happy birthday to me!
CHILD: My process id (PID) is 3817321
goldsd3@linux-new:~/f26/csci2800$ ./a.out 
PARENT: My new child process has PID 3817544
CHILD: Happy birthday to me!
CHILD: My process id (PID) is 3817544
PARENT: My process id (PID) is 3817543
goldsd3@linux-new:~/f26/csci2800$ ./a.out 
PARENT: My new child process has PID 3817550
PARENT: My process id (PID) is 3817549
CHILD: Happy birthday to me!
CHILD: My process id (PID) is 3817550
goldsd3@linux-new:~/f26/csci2800$ ./a.out 
PARENT: My new child process has PID 3817556
CHILD: Happy birthday to me!
CHILD: My process id (PID) is 3817556
PARENT: My process id (PID) is 3817555
goldsd3@linux-new:~/f26/csci2800$ 
#endif

