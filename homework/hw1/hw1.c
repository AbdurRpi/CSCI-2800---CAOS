/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
   
   float value;
   //char *v = value;
   int *size;
   int *index;
   float **cache;

   cache = calloc(64, sizeof(float *));
   if(cache == NULL) {perror("ERROR: calloc() failed"); return EXIT_FAILURE;}

   *(cache + 3) = realloc(index, sizeof(size));
   if(*(cache +3) == NULL) {perror("ERROR: calloc() layer 2 allocation failed"); return EXIT_FAILURE;}
   printf("Enter floating-point values below (CTRL-D to end).");
   atoi(size);
   scanf("%f", &value);

   while(value != "CTRL-D"){
      for (*size = size; *size/abs(value); size++){
         size/abs(value) = index
         (*(*cache + 3) + index);
      }
      for (char **cache = cache; *cache != NULL; cache++ ){
         printf("Value %s hashes to index %d (calloc)\n", value, index);
      }
   }

   free(*(cache + 3));
   free(cache);
   return EXIT_SUCCESS;
}
