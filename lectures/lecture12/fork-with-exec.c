/* fork-with-exec.c */

/* fork() is a system call that creates a new (child) process */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

    sleep( 10 );

    printf( "CHILD: executing ls -l fork.c...\n" );
                    /* argv[0], argv[1],  argv[2], argv[3] */
    execl( "/usr/bin/ls", "ls",    "-l", "fork.c",    NULL );

    /* when we call execl() here, the child process ID (pid) does not change */

#if 0
    printf( "CHILD: executing ls -l nosuchfile.txt...\n" );
                    /* argv[0], argv[1],  argv[2],         argv[3] */
    execl( "/usr/bin/ls", "ls",    "-l", "nosuchfile.txt",    NULL );
#endif

#if 0
    execl( "./a.out", "a.out", NULL );   /****** FORK BOMB ******/
#endif

    /* we should never get to this point in the code...! */
    perror( "execl() failed" );
    return EXIT_FAILURE;
  }
  else /* p > 0 */  /* PARENT process runs this code... */
  {
    printf( "PARENT: My new child process has PID %d\n", p );
    printf( "PARENT: My process id (PID) is %d\n", getpid() );

    /* wait (BLOCK indefinitely) for the child process to end... */
    int status;
    pid_t child_pid = waitpid( p, &status, 0 );
                              /* ^^^
                               * this is pass-by-reference... */

    printf( "PARENT: child process %d terminated...\n", child_pid );

    if ( WIFSIGNALED( status ) )
    {
      printf( "PARENT: ...abnormally (killed by a signal)\n" );
    }
    else if ( WIFEXITED( status ) )
    {
      int exit_status = WEXITSTATUS( status );
      printf( "PARENT: ...normally with exit status %d\n", exit_status );
    }
  }

  return EXIT_SUCCESS;
}
