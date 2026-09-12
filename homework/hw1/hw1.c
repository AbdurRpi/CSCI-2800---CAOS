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
   int size; // size of array
   int index; // index of the array
   float **cache; // array of char cache

   if (argc != 2){
      fprintf(stderr, "ERROR: expected one cache size argument\n");
      return EXIT_FAILURE;
   }
   size = atoi(*(argv +1));

   if (size <= 0){
      fprintf(stderr, "ERROR: cache size must be positive\n");
      return EXIT_FAILURE;
   }

   cache = calloc(size, sizeof(float *));
   if(cache == NULL) {perror("ERROR: calloc() failed"); return EXIT_FAILURE;}

   int *counts = calloc(size, sizeof(int));
   printf("Enter floating-point values below (CTRL-D to end).\n");
   

   while(1){ // If ctrl-d is entered cut program
      int result = scanf("%f", &value);
      if (result == EOF){
         break;
      }
      if ( result == 0){
         char buffer[1024];
         if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            break;
         }
         continue;
      }

      int fpos = -1;
      index = abs((int)value) % size;

      for (int pos = 0; pos < *(counts + index); pos ++){
         if ((*(*cache + index) + pos) == value){
            fpos = pos;
            break;
         }
      }
      if (fpos != -1){
         if (fpos == *(counts + index) - 1){
               printf("Value %.3f Hashes to index %d (nop)\n ", value, index);
         } else{ // Reorder Function
            float lpos = *(*(cache + index)+ fpos); // Last position (lops) to reference a move off of 
            for (int pos = fpos; pos < *(counts + index)-1 ; pos++){
               (*(*(cache + index) + pos) = *(*cache + index) + (pos + 1));
            }
            *(*(cache + index) + (*(*counts + index) - 1)) = lpos;
            printf("Value %.3f hashes to index %d (reorder)\n", value, index);
         }
      }else if (*(counts + index) == 0){ //calloc function
         *(cache + index) = calloc(1, sizeof(float));
         *(*(cache + index)+ 0) = value;
         *(counts + index) = 1;
         printf("Value %.3f hashes to index %d (calloc)\n", value, index);
      }else if (*(counts + index) < 3){ // Realloc function
         float *order = realloc(*(cache + index), (*(counts + index)+ 1 ) * sizeof(float));
         if (order == NULL) {
            perror("Error: realloc() failed\n");
            free((*(cache + index)));
            return EXIT_FAILURE;
         }
         *(cache + index) = order;
         *(*(cache + index) + *(counts + index)) = value;
         (*(cache + index))++;
         printf("Value %.3f hashes to index %d (realloc)\n", value, index);
      }else { // shift position function
         for (int pos = 0; pos < 2; pos ++){
            *(*(cache + index)+ pos) = *(*(cache + index)+ (pos +1));
         }
         *(*(cache + index)+2) = value;
         printf("Value %.3f hashes to index %d (shift)\n", value, index);
      }

      }
   #if 0
      for (counts == 0 ; counts <= *(*(cache + 1)+ 3); counts ++){ 
         index = value % size;// counts if counts is less than 3 spaces on the second layer of the arrays elements
         
            printf("Value %3.3d Hashes to index %d (calloc)/n ");
         }
         for(index > 0; *(*(cache + 1) + index) < *(*(cache + 1) + *(index)); index++){. // attempting to reorder the indexes from greatest to least 
            if ((*(*cache + 1) + index) <= *(*(cache + 1)+ index)){
         } 
         for(int i = 0; i < size; i++){
            free((*(cache + i)));
         }
      }
      #endif
         free(*(counts));
         free(cache);
         return EXIT_SUCCESS;
   
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
   


