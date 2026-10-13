/* fork-pass-by-fork.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHUNK_SIZE 8

void block_on_waitpid( pid_t pid );
void child_process( char * data, int i );

int main()
{
  char * data = calloc( 27, sizeof( char ) );

  /*                       1111111111222222                       */
  /*             01234567890123456789012345  *(data + 26) == '\0' */
  strcpy( data, "abcdefghijklmnopqrstuvwxyz" );

  int children = 0;

  /* index variable i indicates to the child process which
   *  chunk of data to convert to uppercase and display;
   *   each child process works on a substring of length CHUNK_SIZE
   */
  for ( int i = 0 ; i < strlen( data ) ; i += CHUNK_SIZE )
  {
    /* create the next child process */
    pid_t p = fork();  /* i = 0, 8, 16, 24, 32 */
    if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }

    if ( p == 0 ) /* CHILD */
    {
      child_process( data, i );  /* pass-by-fork occurs here */
      free( data );              /* avoid a memory leak in each child process */
      return EXIT_SUCCESS;
    }

    /* PARENT */
    children++;
    printf( "PARENT: my new child process has PID %d\n", p );
#if 0
    block_on_waitpid( p );    /* this will run child processes sequentially */
#endif
  }

#if 1
  while ( children > 0 )   /* run the child processes in parallel */
  {
    block_on_waitpid( -1 );   /* wait for each child process to terminate */
    children--;
  }
#endif

  printf( "PARENT: all child processes have terminated\n" );
  printf( "PARENT: data: \"%s\"\n", data );

  free( data );

  return EXIT_SUCCESS;
}


void block_on_waitpid( pid_t pid )
{
  /* wait (BLOCK indefinitely) for the child process to end... */
  int status;
  pid_t child_pid = waitpid( pid, &status, 0 );
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

void child_process( char * data, int i )
{
sleep( 2 );
  for ( int j = 0 ; j < CHUNK_SIZE ; j++ )  /* TO DO: BUFFER OVERFLOW BUG... */
  {
    if ( *(data+i+j) && isalpha( *(data+i+j) ) )   /* or islower() */
    {
      *(data+i+j) = toupper( *(data+i+j) );
    }
  }
  printf( "CHILD %d: data: \"%s\"\n", getpid(), data );
}

/* BUG:  for the last child_process() call, e.g., "YZ\0" */





