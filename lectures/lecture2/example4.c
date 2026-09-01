/* dynamic memory allocation example*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
   /* path is static allocation (on the stack)*/
   /* dynamically allocate 16 bytes on the runtime heap*/
   /* ^^^ Dyanmic happens at RUNTIME ^^^*/
   char * path = malloc( 16 );

   /* Note that malloc() returns a void* (void pointer), which 
   is essentially a placeholder pointer, i.e., a memory address*/

   if ( path == NULL){
      /* perror(): human-readable translation of errno */
      perror("malloc() failed");
      return EXIT_FAILURE;
   }

   printf("sizeof ( path ) is %lu\n", sizeof(path));
   strcpy( path, "/home/abdur/CAOS/lectures/lecture2");
   printf("path is \"%s\" (strlen is %lu)\n", path);

   /* valgrind or drmemory - to preform dynamic memory checks ./a.out*/
   free( path );
   return EXIT_SUCCESS;
}