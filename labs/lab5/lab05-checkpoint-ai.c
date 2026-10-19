/* lab05-checkpoint1-ai.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int pipefd[2];
  pid_t p;
  unsigned short value = -1;
  pipe( pipefd );
  p = fork();

  if ( p > 0 )
  {
    while ( value != 0 )
    {
      printf( "Enter positive integer to send on the pipe (0 to exit): " );
      if ( scanf( "%hu", &value ) == 1 )
      {
        if ( value > 0 )
        {
          if ( write( pipefd[1], &value, 4 ) > 0 )
          {
            printf( "Okay, wrote %hu to the pipe\n", value );
          }
        }
      }
    }
  }
  else
  {
    while ( value != 0 )
    {
      if ( read( pipefd[0], &value, 4 ) > 0 )
      {
        printf( "Read %hu from the pipe in the child process\n", value );
      }
    }
  }

  close( pipefd[0] );
  close( pipefd[1] );

  return EXIT_SUCCESS;
}