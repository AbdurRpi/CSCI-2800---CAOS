/* check 1*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){
   
   int x,y,z;
   int s = (scanf("%d %d %d", &x, &y, &z) != 3);

   printf("Enter three integers in non-descending order:\n");
   if(s) goto LABEL1;


   
   int correct = 0;
   
   // For parent process
   int first = x;
   int second = y;
   int third = z;
   int temp;
   int check1 = (first > second);
   int check2 = (second > third);
   int check3 = (first > second);

   int order1 = (x == first);
   int order2 = (y == second);
   int order3 = (z == third);

   int status;
   pid_t p = fork();
   pid_t t = waitpid(p, &status, 0);

   int ec = WEXITSTATUS(status);
   int sn = WTERMSIG(status);

   int a = (p == -1);
   int b = (p > 0); // for parent process
   int c = (p == 0); // for child process
   int e = ( t == -1);

   if (a) goto LABEL2; // error check if invalid value
   if (b) goto LOOP1; // if parent process
   if (c) goto LOOP2; // if child process

 
   LOOP1:
      if(check1){
         temp = first;
         first = second;
         second = temp;
      }
         
      if(check2){
         temp = second;
         second = third;
         third = temp;
      }
         
      if(check3){
         temp = first;
         first = second;
         second = temp;
      } 

      goto LABEL6;

      if (order1) correct ++;
      if (order2) correct ++;
      if (order3) correct ++;

      exit(correct);
      goto DONE1;
   DONE1:

   LOOP2:
      if (e) goto LABEL3;
      if(WIFEXITED(status)) goto LABEL4;
         
      else if (WIFSIGNALED(status)) goto LABEL5;
      goto DONE2;
   DONE2:

   LABEL1:
      fprintf(stderr, "ERROR: Invalid input\n");
      return EXIT_FAILURE;

   LABEL2:
      perror("fork failed");
      return EXIT_FAILURE;
   
   LABEL3:
      perror("waitpid failed");
      return 1;
   
   LABEL4:
      printf("PARENT: child process reported %d in the correct position\n",ec );
   
   LABEL5:
      printf("PARENT: child terminated by signal %d\n", sn);

   LABEL6:
      printf("CHILD: correct order is %d %d %d\n",first, second, third);
   
   return EXIT_SUCCESS;
}
