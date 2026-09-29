/* compile via:
Bash$ gcc -Wall -Werror check3.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, **argv){

int x,y,z;

printf("Enter three integers in non-descending order:\n");
scanf("%d,%d,%d" x, y, z);

if (argc != 3){
   fprintf(stderr, "ERROR: expected argument\n");
   return EXIT_FAILURE;
}
int b = 0;
pid_t p = fork();

if (p == 0){
   if (x < y && y <= z && x < z){
      x += 3
      printf("CHILD: correct order is %d %d %d" x, y, z);
   }else if(x > y || x > z){
      x += 0

   }
      

   }
   return EXIT_SUCCESS;
}

if (p => 0){
   printf(PARENT:)
}

}