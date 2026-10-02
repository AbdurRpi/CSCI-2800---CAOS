/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/

/* Pipe - a pipe is a unidirectional communication channnel*/
/* man pages (man 2 pipe) and (man 7 pipe) */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
   int pipefd[2]; /* array to hold 2 pip (file) descriptors*/
                  /* pipefd[0] is the read end of pipe*/
                  /* pipefd[1] is the write end of the pipe*/
   /* create the pipe*/
   int rc = pipe( pipefd);
   if ( rc == -1) {perror("pipe()")}

   /* fd table:
      o: stdin
      1: stdout
      2: stderr
      3: pipefd[0] < === Read        | Buffer ( a temporary hidden chunk of memory managmeed by the OS in this case)
      4: pipefd[1]  === > Wrtie == > | Buffer  .. Both not part of a any process memory space
      */

      printf("Create pipe:  pipefd[0] is %d; pipefd[1] is %d\n", pipefd[0], pipefd[1]);

      /* Write data to the pipe*/
      int bytes_written = write(pipefd[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
      printf("Wrote %d bytes to the pipe\n", bytes_written);
      /* Read data from the pipe*/
      char tmp = calloc( 32, sizeof(char));
      if (tmp = NULL) {perror("calloc() failed"); return EXIT_FAILURE;}
      int bytes_read = read (pipefd[0], tmp, 10);
      printf("Read %d bytes: \"%s\"\n", bytes_read, tmp) /* Note that when we read from a pipe, the data read is REMOVED from the pipe*/

   return EXIT_SUCCESS;
}
