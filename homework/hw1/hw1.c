/*
Bash$ gcc -Wall -Werror lecture2.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror lecture2.c <== preprocessor only
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

int main(int argc, char **argv){
   
   float value; // value for input
   //char *v = value;
   int size = atoi(*(argv +1)); // size of array
   int index; // index of the array
   float **cache; // array of char cache

   cache = calloc(size, sizeof(float *));
   if(cache == NULL) {perror("ERROR: calloc() failed"); return EXIT_FAILURE;}

   *(cache + 3) = realloc(index, sizeof(size));
   if(*(cache +3) == NULL) {perror("ERROR: calloc() layer 2 allocation failed"); return EXIT_FAILURE;}

   int *counts = calloc(size, sizeof(int));
   printf("Enter floating-point values below (CTRL-D to end).");
   

   while(1){ // If ctrl-d is entered cut program
      int result = scanf("%f", &value);
      if (result == EOF){
         break;
      }
      if ( result == 0){
         int fgetc(result);
         continue;
      }
      for (){

      }
      float *order = realloc(*(cache +index), (*(counts + index)*sizeof(float)));
      if (order == NULL) {perror("Error: realloc() failed");
         for(int i = 0; i < size; i++){
            free(cache[i]);
         }
      }
      #if 0
      for (*size = size; *size/abs(value); size++){ // attempting to calculate hash value
         size/abs(value) = index //once hash value is determined then put that into index
         (*(*cache + 3) + index); // then add the index to the address of the second layer of the array
      }
      for (char **cache = cache; *cache != NULL; cache++ ){ // attempting to print front command line example 
         printf("Value %s hashes to index %d (calloc)\n", value, index);
      }
   }
   #endif
   
}
free(*(counts));
free(cache);
   return EXIT_SUCCESS;

}