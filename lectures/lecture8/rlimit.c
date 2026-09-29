/* rlimit.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

/* bash$ ulimit -a      <== view the current user's resource limits...  */

int main()
{
  struct rlimit rl;
  getrlimit( RLIMIT_NPROC, &rl );
  printf( "RLIMIT_NPROC soft limit: %ld\n", rl.rlim_cur );
  printf( "RLIMIT_NPROC hard limit: %ld\n", rl.rlim_max );

  /* lower the RLIMIT_NPROC soft limit to 32 */
  rl.rlim_cur = 32;
  setrlimit( RLIMIT_NPROC, &rl );   /* TO DO: check return value */

  getrlimit( RLIMIT_NPROC, &rl );
  printf( "RLIMIT_NPROC soft limit: %ld\n", rl.rlim_cur );
  printf( "RLIMIT_NPROC hard limit: %ld\n", rl.rlim_max );

#if 1
  while ( 1 )
  {
    pid_t p = fork();
    if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }
    printf( "PID %d: fork() worked\n", getpid() );

    sleep( 3 );   /* delay/suspend each process for 3 seconds.... */
  }
#endif

  return EXIT_SUCCESS;
}
