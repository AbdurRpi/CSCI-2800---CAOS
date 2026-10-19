/* signals.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* Signals give us an asynchronous means of inter-process communication (IPC) */

/* TO DO: check out "man 7 signal" and also "man 2 signal" */

void signal_handler( int sig )
{
  printf( "Rcvd signal %d\n", sig );
  if ( sig == SIGINT ) printf( "Stop hitting CTRL-C!\n" );
  else if ( sig == SIGTERM ) printf( "Sorry, I'm not shutting down\n" );

  /* e.g., SIGUSR1 causes the (server) process to re-load its config file... */
}

int main()
{
  signal( SIGINT, SIG_IGN );   /* ignore SIGINT (CTRL-C) */
  signal( SIGTERM, SIG_IGN );  /* ignore SIGTERM */

  signal( SIGINT, signal_handler );  /* SIGINT will cause signal_handler() function to run */
  signal( SIGTERM, signal_handler ); /* SIGTERM will cause signal_handler() function to run */

  char * name = calloc( 128, sizeof( char ) );
  printf( "Enter name: " );
  scanf( "%s", name );   /* buffer overflow is possible here... */
  printf( "Hi, %s\n", name );

#if 0
  signal( SIGINT, SIG_DFL );   /* restore default behavior for SIGINT (CTRL-C) */
  sleep( 10 );
#endif

  free( name );
  return EXIT_SUCCESS;
}
