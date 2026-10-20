/* lecture 14*/


// MIPS

/* to generate assembly code, use -s flag in compliation*/
/* bash$ gcc -s calculate.c*/

/* install SPIM: bash$ sudo apt-get install spim */

// REMAKE this in MIPS: 

// from calculate.c code

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
   int g = 7, h = 6, i = 5, j = 2;

   int f = g + h - i - j;

   int p = g - f;
   printf("%d", p);
}

