/* compile via:
Bash$ gcc -Wall -Werror check2.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <ctype.h>

#if 0
void finder(char n){
   for(n)
   }
   return fibn(n -1) + fibn(n - 2);
      }
#endif
int main(int argc, char **argv){

   
   
   if (argc != 2){ 
      fprintf(stderr, "ERROR: expected argument\n");
      return EXIT_FAILURE;
   }

   int fd = open(*(argv + 1), O_RDONLY);

   if(fd == -1){
      perror("ERROR: open() failed");
      return EXIT_FAILURE;
   }
   signed char byte;
   ssize_t br;

   
   
   while((br = read(fd, &byte, 1)) > 0){
      unsigned int c = (unsigned char)byte;
      if (byte == '\n'){
         printf("Char '\\n' ==> decimal %d; octal 0%o; hex 0x%x\n",(int)byte, c, c);

      }else if(isprint((unsigned char)byte)){
         printf("Char '%c' ==> decimal %d; octal 0%o; hex 0x%x\n", byte, (int)byte, c, c);
     
      }else{
         printf("Char 'non-printable' ==> decimal %d; octal 0%o; hex 0x%x\n",(int)byte, c, c);
      }
      
   
}
if (br == -1){
         perror("ERROR: open() failed");
      return EXIT_FAILURE;
      }
      if ((close(fd))== -1){
         perror("ERROR: open() failed");
      return EXIT_FAILURE;
   }
return EXIT_SUCCESS;
}
 #if 0
   if(scanf("%d", &i) != 1){
      perror("Error: invalid input\n");
      return EXIT_FAILURE;
      //printf("%d\n", n);
   }
   if ( i == -1){
      break;
   }
   if ( i < 0){
      continue;
   }
   #endif
   //printf("Char %c ==> decimal %d; octal %o; hex %x", chars, chars, chars, chars);
   //printf("Using unsigned long, fib(%d) is %lu.\n", i, fibl((unsigned int )i));




