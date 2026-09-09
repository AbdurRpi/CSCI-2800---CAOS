#if 0

size_t capacity = <small starting number>;
size_t length = 0;
char *buffer = calloc(capacity, sizeof(char));

while (not EOF) {
    read a char with fgetc(stdin)
    if alphanumeric:
        if (length == capacity) {
            // grow the buffer
            size_t old_capacity = capacity;
            capacity *= 2;
            buffer = realloc(buffer, capacity);
            // zero out the NEW portion with memset, since realloc doesn't do it for you
        }
        store character, length++
}

if (length == capacity) {
    size_t old_capacity = capacity;
    capacity *= 2;
    buffer = realloc(buffer, capacity);
    if (buffer == NULL) {
        perror("realloc failed");
        return EXIT_FAILURE;
    }
    memset(buffer + old_capacity, 0, capacity - old_capacity);
}
#endif
/*
Try again with these fixes:

Fix the while condition to only check the thing that still makes sense to check (one condition, not two)
Fix the colon → brace
Add the missing semicolon
Add the "store character + increment length" lines after the growth-check block
Move return EXIT_SUCCESS out of the loop entirely, down to where printf/free will go
Make sure every { has a matching }

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#if 0
while (length <= capacity){
      c = fgetc(stdin);
   if (c ==EOF){
     break;
   }
   if (isalnum(c)){
   if(length == capacity){
      size_t old_capacity = capacity;
      capacity *=2
      buffer = realloc(buffer, capacity);
   }

   if (buffer == NULL){
   perror("realloc failed");
    return EXIT_FAILURE;
}
memset(buffer + old_capacity, 0, capacity - old_capacity);

}
return EXIT_SUCCESS;
}
#endif

int main(){
   size_t capacity = 16;
   //size_t o_capacoty = capacity;
   char *buffer = calloc(capacity, sizeof(char));
   if (buffer == NULL){perror("calloc() failed");
      return EXIT_FAILURE;
   }
      
   int count = 0;
   int c;
   while(count < capacity){
       int duplicate = 0;
      c = fgetc(stdin);
      if (c == EOF){
            break;
      }
      for(int i = 0; i < count; i++){
         if( (char) c == *(buffer + i)){
            duplicate = 1;
            break;
         }
      }
      if ( duplicate ==1){
         continue;
      }
      if(c == ((char) c)){
            duplicate = 1;
         }
      if (isalnum(c)){
         *(buffer + count) = (char) c;
         count++;
      }
   }
   printf("%s\n", buffer);
   free(buffer);
   return EXIT_SUCCESS;

}



