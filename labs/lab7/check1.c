/* check 1*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){
   int x,y,z;

   printf("Enter three integers in non-descending order:\n");

   if(scanf("%d %d %d", &x, &y, &z) != 3) goto LABEL1;

   LABEL1:
      fprtinf(stderr, "ERROR: Invalid input\n");
      return EXIT_FAILURE;

   LABEL2:





   LABEL3:



























}
