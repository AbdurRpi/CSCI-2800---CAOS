/* check 1*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){
   
   int x,y,z;
   int s;
   int bad;

   //goto LABEL7;
   printf("Enter three integers in non-descending order:\n");
   s = (scanf("%d %d %d", &x, &y, &z));
   bad = (s != 3);
   if(bad) goto LABEL1;


   
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
   pid_t t; 

   int ec;
   int sn;

   int a = (p == -1);
   int b = (p > 0); // for parent process
   int c = (p == 0); // for child process
   int e;

   if (a) goto LABEL2; // error check if invalid value
   if (b) goto LOOP2; // if parent process
   if (c) goto LOOP1; // if child process

 
   LOOP1: // CHILD loop
      if(check1){ // Change all loop conditions to not use  {} and to make check 2 and 3 properly check as they are outdated
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
      
      order1 = (x == first);
      order2 = (y == second);
      order3 = (z == third);

      correct = correct + order1;
      correct = correct + order2;
      correct = correct + order3;

      printf("CHILD: correct order is %d %d %d\n",
             first, second, third);
      exit(correct);
      
      //goto LABEL4;
      //goto DONE1;
   //DONE1:

   LOOP2: // Parent loop
      t = waitpid(p, &status, 0);
      e = ( t == -1);
      if (e) goto LABEL3;
      if(WIFEXITED(status)) goto LABEL4;
         
      else if (WIFSIGNALED(status)) goto LABEL5;
      //goto DONE2;
   //DONE2:

   LABEL1: // error statement
      fprintf(stderr, "ERROR: Invalid input\n");
      return EXIT_FAILURE;

   LABEL2: // error statemnt
      perror("fork failed");
      return EXIT_FAILURE;
   
   LABEL3: // error statment
      perror("waitpid failed");
      return 1;
   
   LABEL4: // output statment
      ec = WEXITSTATUS(status);
      printf("PARENT: child process reported %d in the correct position\n",ec );
      return EXIT_SUCCESS;
   
   LABEL5: // error check statament
      sn = WTERMSIG(status);
      printf("PARENT: child terminated by signal %d\n", sn);

   //LABEL7: // Print statement
      
      //goto LOOP1;
   
   return EXIT_SUCCESS;
}
