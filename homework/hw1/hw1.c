/*
Bash$ gcc -Wall -Werror hw1.c
Bash$ gcc -Wall -Werror lecture2.c -lm
Bash$ gcc -E -Wall -Werror hw1.c <== preprocessor only
*/

/* I used pointer arithmetic instead of [ ] so some functions look big  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

int main(int argc, char **argv){ // using argc for command line strings and argv points to thise strings in array
   
   float value; // value for input
   //char *v = value;
   int size; // size of array
   int index; // index of the array
   float **cache; // array of char cache

   if (argc != 2){ // Program only needs 1 argument after the program name so it needs to be = 2 if not represent error
      fprintf(stderr, "ERROR: expected one cache size argument\n");
      return EXIT_FAILURE;
   }
   size = atoi(*(argv +1)); // converts cache string into an integer

   if (size <= 0){ // cache size has to be positive, a zero value is an inlavid argurment or an entry of 0
      fprintf(stderr, "ERROR: cache size must be positive\n");
      return EXIT_FAILURE;
   }

   cache = calloc(size, sizeof(float *)); // creates the first layer of the cache array
   if(cache == NULL) {perror("ERROR: calloc() failed"); return EXIT_FAILURE;}

   int *counts = calloc(size, sizeof(int)); // creates a parrallel integer array to keep track of floats stored at each index
   printf("Enter floating-point values below (CTRL-D to end).\n");
   

   while(1){ // input valudation - reads until EOF
      int result = scanf("%f", &value); 
      if (result == EOF){
         break;
      }
      if ( result == 0){ // Stop if EOF occurs whil invalid is being removed from input
         int buffer = fgetc(stdin); // buffer is the name i used to check invalid inputs 
         if (buffer == EOF){
             break;
         }
           continue;
         }
         
      

      int fpos = -1; // stands for found position of the value in the cache index
      index = abs((int)value) % size; // calculates the remainder for indexing below - which calculates the hashe table index 

      for (int pos = 0; pos < *(counts + index); pos ++){ // position of a value that was indexed into the array 
         if (*(*(cache + index) + pos) == value){
            fpos = pos;
            break;
         }
      }
      if (fpos != -1){ // Checks wether value is already in the cache and if so (meaning its not -1) then it either readorders or nop
         if (fpos == *(counts + index) - 1){
               printf("Value %.3f hashes to index %d (nop)\n", value, index);
         } else{ // Reorder Function
            float lpos = *(*(cache + index)+ fpos); // stands for Last position (lops) which is a reference to move off of since its stored oldest to newest, its already in the most recently used value
            for (int pos = fpos; pos < *(counts + index) - 1 ; pos++){
               *(*(cache + index) + pos) = *(*(cache + index) + (pos + 1));
            }
            *(*(cache + index) + (*(counts + index) - 1)) = lpos;
            printf("Value %.3f hashes to index %d (reorder)\n", value, index);
         }
      }else if (*(counts + index) == 0){ //calloc function  - if 0 then it means that the value was not found and the cache entry is empty
         *(cache + index) = calloc(1, sizeof(float));
         *(*(cache + index)+ 0) = value;
         *(counts + index) = 1;
         printf("Value %.3f hashes to index %d (calloc)\n", value, index);
      }else if (*(counts + index) < 3){ // Realloc function - reallocates more memory when called In the case that a value was not found but the entry contains fewer than 3 values, so it exapnds the 2nd layer array by 1 float.
         float *order = realloc(*(cache + index), (*(counts + index)+ 1 ) * sizeof(float));
         if (order == NULL) {
            perror("Error: realloc() failed\n");
            free((*(cache + index)));
            return EXIT_FAILURE;
         }
         *(cache + index) = order;
         *(*(cache + index) + *(counts + index)) = value;
         (*(counts + index))++;
         printf("Value %.3f hashes to index %d (realloc)\n", value, index);
      }else { // shift position function
         for (int pos = 0; pos < 2; pos ++){ // Will shift the indexes of a value from newest to oldest and the correpsonding value index order
            *(*(cache + index)+ pos) = *(*(cache + index)+ (pos +1));
         }
         *(*(cache + index)+2) = value;
         printf("Value %.3f hashes to index %d (shift)\n", value, index);
      }
      

      } // Function to print out the visualization of the first layer of cache entries in assending order after input is complete
      for (int i = 0; i < size; i++){
         if(*(counts + i) > 0){
            printf("[%d] ==> ", i);

            for (int j = 0; j < *(counts + i); j++){
               if(j == 0){
                  printf("%.3f", *(*(cache + i) + j));
               } else{
                  printf(", %.3f", *(*(cache + i) + j));
               }
            }
            printf("\n");
         }
      }
      for (int i = 0; i < size; i++){ // Dynamically frees memory from second layer and from each allocation
         free(*(cache + i));
      }
         free(counts);
         free(cache);
         return EXIT_SUCCESS;
   
         }
         

      
      #if 0

      JUST old testing code that didnt work :(
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

      for (*size = size; *size/abs(value); size++){ // attempting to calculate hash value
         size/abs(value) = index //once hash value is determined then put that into index
         (*(*cache + 3) + index); // then add the index to the address of the second layer of the array
      }
      for (char **cache = cache; *cache != NULL; cache++ ){ // attempting to print front command line example 
         printf("Value %s hashes to index %d (calloc)\n", value, index);
      }
   }
   #endif
   


