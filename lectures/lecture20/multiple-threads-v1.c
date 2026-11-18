/* multiple-threads-v1.c */

/* Within exactly ONE process, we can have multiple
 *  threads of execution running...
 *
 * We always start with one thread, i.e., the main thread
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* thread function --- each thread will run this code... */
void * whattodo( void * arg );

int main( int argc, char ** argv )
{
  srand( time( NULL ) * getpid() );

  if ( argc != 2 )
  {
    fprintf( stderr, "ERROR: invalid arguments\n" );
    fprintf( stderr, "USAGE: %s <#-of-child-threads>\n", *(argv+0) );
    return EXIT_FAILURE;
  }

  int children = atoi( *(argv + 1) );

  /* array of thread IDs (tids) for each thread we create */
  pthread_t * tids = calloc( children, sizeof( pthread_t ) );

  int i, rc;

  for ( i = 0 ; i < children ; i++ )
  {
#if 0
    int t = 10 + ( rand() % 21 );   /* [10,30] seconds */
printf( "MAIN: the value of t is %d\n", t );
    rc = pthread_create( tids + i, NULL, whattodo, &t );
#else
    /* dynamically allocate an int to pass into each thread... */
    int * t = malloc( sizeof( int ) );
    *t = 10 + ( rand() % 21 );   /* [10,30] seconds */
    rc = pthread_create( tids + i, NULL, whattodo, t );
#endif
    if ( rc != 0 )
    {
      fprintf( stderr, "MAIN: ERROR: pthread_create() failed (%d)\n", rc );
    }
  }

  /* join all child threads, i.e., block until each child thread
   *  terminates --- the main thread acknowledges each child thread's
   *   termination event
   */
  for ( i = 0 ; i < children ; i++ )
  {
    /* the first argument specifies which child thread to join in
     *  -- there is no catch-all value to use here akin to waitpid( -1, ... )
     */
#if 0
    pthread_join( *(tids + i), NULL );    /* BLOCKING CALL */
    printf( "MAIN: joined a child thread\n" );
#else
    char * result;
    pthread_join( *(tids + i), (void **)&result );    /* BLOCKING CALL */
    printf( "MAIN: joined a child thread that returned \"%s\"\n", result );
    free( result );
#endif
  }

  printf( "MAIN: exiting...\n" );
  free( tids );
  return EXIT_SUCCESS;
}

void * whattodo( void * arg )
{
  int t = *(int *)arg;
  /*       ^^^^^^^ type case -- treat arg as a pointer to an int */
  free( arg );

  printf( "THREAD %lu: Counting down from %d...\n", pthread_self(), t );
  while ( t > 0 )
  {
    printf( "THREAD %lu: %d\n", pthread_self(), t-- );
    sleep( 1 );
  }
  printf( "THREAD %lu: done!\n", pthread_self() );
#if 0
  return NULL;   /* pthread_exit( NULL ); */
#else
  /* return a value from the thread */
  char * s = calloc( 5, sizeof( char ) );
  sprintf( s, "ABCD" );
  return s;      /* pthread_exit( s ); */
#endif
}

