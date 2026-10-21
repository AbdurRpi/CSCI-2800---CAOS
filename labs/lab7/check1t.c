#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
   int x, y, z;
   int count, bad, child;
   int first, second, third, temp;
   int check1, check2, check3;
   int order1, order2, order3, correct;
   int status, exited, signaled, ec, sn;
   pid_t p, t;

   printf("Enter three integers in non-descending order:\n");
   

   count = scanf("%d %d %d", &x, &y, &z);
   bad = (count != 3);
   if (bad) goto INPUT_ERROR;

   p = fork();
   bad = (p == -1);
   if (bad) goto FORK_ERROR;

   child = (p == 0);
   if (child) goto CHILD;

   /* Only the parent reaches this section. */
   t = waitpid(p, &status, 0);
   bad = (t == -1);
   if (bad) goto WAIT_ERROR;

   exited = WIFEXITED(status);
   if (exited) goto NORMAL_EXIT;

   signaled = WIFSIGNALED(status);
   if (signaled) goto SIGNAL_EXIT;

   return EXIT_FAILURE;

CHILD:
   first = x;
   second = y;
   third = z;

   /* First comparison: swap first and second if needed. */
   check1 = (first <= second);
   if (check1) goto CHECK_SECOND;

   temp = first;
   first = second;
   second = temp;

CHECK_SECOND:
   /* Second comparison: swap second and third if needed. */
   check2 = (second <= third);
   if (check2) goto CHECK_FIRST_AGAIN;

   temp = second;
   second = third;
   third = temp;

CHECK_FIRST_AGAIN:
   /* Third comparison: check first and second again. */
   check3 = (first <= second);
   if (check3) goto COUNT_POSITIONS;

   temp = first;
   first = second;
   second = temp;

COUNT_POSITIONS:
   order1 = (x == first);
   order2 = (y == second);
   order3 = (z == third);

   correct = order1;
   correct = correct + order2;
   correct = correct + order3;

   printf("CHILD: correct order is %d %d %d\n",
          first, second, third);
   exit(correct);

NORMAL_EXIT:
   ec = WEXITSTATUS(status);
   printf("PARENT: child process reported %d in the correct position\n",
          ec);
   return EXIT_SUCCESS;

SIGNAL_EXIT:
   sn = WTERMSIG(status);
   printf("PARENT: child terminated by signal %d\n", sn);
   return EXIT_SUCCESS;

INPUT_ERROR:
   fprintf(stderr, "ERROR: Invalid input\n");
   return EXIT_FAILURE;

FORK_ERROR:
   perror("fork failed");
   return EXIT_FAILURE;

WAIT_ERROR:
   perror("waitpid failed");
   return EXIT_FAILURE;
}