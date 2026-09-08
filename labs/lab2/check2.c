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

int main(){
   char *string ;
   string = calloc(16, sizeof(string));
   scanf("%c", string);
   fgetc(string);

   if(string == isalnmun(string); string++);




}