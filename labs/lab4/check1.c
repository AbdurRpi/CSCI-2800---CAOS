/* compile via:
Bash$ gcc -Wall -Werror check3.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){

int x,y,z;

printf("Enter three integers in non-descending order:\n");
//scanf("%d %d %d", &x, &y, &z);

if (scanf("%d %d %d", &x, &y, &z) != 3){
   fprintf(stderr, "ERROR: Invalid input\n");
   return EXIT_FAILURE;
}

//int b = 0;
pid_t p = fork();

if (p == -1){
   perror("fork failed");
   return EXIT_FAILURE;
}

if (p == 0){
      int first = x;
      int second = y;
      int third = z;
      int temp;

      if (first > second){
         temp = first;
         first = second;
         second = temp;
      } if (second > third){
         temp = second;
         second = third;
         third = temp;
      } if (first > second){
         temp = first;
         first = second;
         second = temp;
      }

      printf("CHILD: correct order is %d %d %d\n",first, second, third);

      int correct = 0;

      if (x == first) {
         correct++;
      }if (y == second){
         correct++;
      }if (z == third){
         correct++;
      }
      exit(correct);
#if 0
   if (x < y && y <= z && x < z){
      printf("CHILD: correct order is %d %d %d\n", x, y, z);
      b += 3;
      exit(3);
   }else if(x > y || x > z){
      if (y >= z){
         printf("CHILD: correct order is %d %d %d\n", y, z, x);
         b += 0;
      } 
      exit(0);
   }else{
      if (x == y || y == z){
         printf("CHILD: correct order is %d %d %d\n", z, y, x);
         b += 1;
      }
      exit(1);
   }
#endif
   }

if (p > 0){
   int status;
   pid_t t = waitpid(p, &status, 0);

   if (t == -1){
      perror("waitpid failed");
      return 1;
   }

   if(WIFEXITED(status)){
      int ec = WEXITSTATUS(status);
      printf("PARENT: child process reported %d in the correct position\n",ec );
   }else if(WIFSIGNALED(status)){
      int sn = WTERMSIG(status);
      printf("PARENT: child terminated by signal %d\n", sn);
   }
      
}
return EXIT_SUCCESS;
}