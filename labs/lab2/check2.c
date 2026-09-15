/* Check 2*/
/*
Bash$ gcc -Wall -Werror check2.c
Bash$ gcc -Wall -Werror check2.c -lm
Bash$ gcc -E -Wall -Werror check1.c <== preprocessor only
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#if 0
int main(int st, char **string){
   
   string = calloc(16, sizeof(string));
   scanf("%c", string);
   

   if(string == isalnum(char *); string != NULL; *(string + 1)){
      fgetc(*(string + st));
   }
      else{
         perror("String is not alphanumerical");
         return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
   }
      #endif
      #if 0
int main( int argc, char ** argv){
   printf("argc is %d\n", argc);

   char * name = "check2test.txt";

   int fd = open(name, O_RDONLY);

   if( fd == -1){ perror("openfailed");}

   argv = calloc(16, sizeof(char *));

   buffer[32];
   while (argc <= 16;){
      int rc = read(fd, buffer, 16);
      if ( isalnum(argc)){
         if ( rc == 0) break;
      break[rc] = '\0'
      fgetc(argc);
      }
      free(buffer[rc]);
   }

return EXIT_SUCCESS;

}

#endif

int main(){
   char *buffer = calloc(16, sizeof(char));
   if (buffer == NULL){perror("calloc() failed");
      return EXIT_FAILURE;
   }
      
   int count = 0;
   int c;
   while(count < 16){
      c = fgetc(stdin);
      if (c == EOF){
         break;
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





