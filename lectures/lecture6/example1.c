/* checking processes */

/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/
/*
ps -ef grep

*/

/* This program explains what a fork does*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
   int x = 5;
   pid_t p; /* pid_t is esssentially unsigned int*/

   p = fork(); // attempt to create a new (child) process, which is a duplicate or this parent process

   if (p == -1) { 
      perror("fork() failed"); 
      return EXIT_FAILURE;
   }

   /* we now have two seperate running a.out processes */

   if (p == 0){ /* child process runs this code */
      x += 100;
      printf("CHILD: Happy Birthday to me! x is %d\n", x );
      printf("CHILD: My process id (PID) is %d\n", getpid() );
      printf("CHILD: My parent process id (PID) is %d\n", getpid() );
               /* TO DO: also check getppid()*/
   } 
   else{/* p > 0*/ /* Parent process runs this code*/
      x += 15;
      printf("PARENT: My new child process has PID %d; x is %d\n", p, x);
      printf("PARENT: My process id (PID) is %d\n", getpid());
   } 
   /* BOTH parent and child process will continue running code here...*/
   free( name );
   
   return EXIT_SUCCESS;
}

