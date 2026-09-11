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
   
   float value; // value for input
   //char *v = value;
   int *size; // size of array
   int *index; // index of the array
   float **cache; // array of char cache

   cache = calloc(64, sizeof(float *));
   if(cache == NULL) {perror("ERROR: calloc() failed"); return EXIT_FAILURE;}

   *(cache + 3) = realloc(index, sizeof(size));
   if(*(cache +3) == NULL) {perror("ERROR: calloc() layer 2 allocation failed"); return EXIT_FAILURE;}
   printf("Enter floating-point values below (CTRL-D to end).");
   atoi(size);
   scanf("%f", &value);

   while(value != "CTRL-D"){ // If ctrl-d is entered cut program
      for (*size = size; *size/abs(value); size++){ // attempting to calculate hash value
         size/abs(value) = index //once hash value is determined then put that into index
         (*(*cache + 3) + index); // then add the index to the address of the second layer of the array
      }
      for (char **cache = cache; *cache != NULL; cache++ ){ // attempting to print front command line example 
         printf("Value %s hashes to index %d (calloc)\n", value, index);
      }
   }

   free(*(cache + 3));
   free(cache);
   return EXIT_SUCCESS;
}
