/* compile via:
Bash$ gcc -Wall -Werror simple.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 

COMPILE VIA: gcc -Wall -Werror lab1.c -o lab1 -lm 
THEN: ./lab1

Always look at man(ual) pages for functions
- man printf
- man 3 printf <- gives library functions
- man 2 printf >- gives system calls*/



/* Check Point 2 write a program that displays a table of data showing the natural logrithm and exponential functions with base e of powers 2 ranging from 2^0 to 2^7 */


#include <stdio.h>
#include <math.h>
int x;

int main(){

   double logn, expn;

   printf("                       x\n");
   printf("  x  ln(x)            e\n");
   printf("---  ---------------  ------------------\n");
   for (int x = 1; x <= 256; x*=2){
      expn = exp(x);
      logn = log(x);
      printf("%3d  %.13f  %.13f\n", x, logn, expn);
   }    
   printf("---  ---------------  ------------------\n");
   return 0;
   


}


/* Check Point 3*/
/*
#include <stdio.h>
#include <math.h>

int n;


void triangle(int n){
for(int x = n; x >= 1; x--){
   for(int y = 0; y < n - y - 1; y++){
         printf("  ");
   }
      for(int z = 0; z < (2 * x - 1); z++){
         printf("^");
      }
         printf("\n");
   }
   }
*/
/*
void triangle(int n){
   for(int r = 0; r < (n+1) / 2; r++){
      for(int s = 0; s < r ; s++){
          printf(" ");
      }
      for (int c = 0; c < n -2*r; c++){
         printf("^");
      }
      printf("\n");
      }
      
}

int main(void){

   //char v = "^";

while(n != -999){
   printf("Enter positive integer (-999 to exit): \n");
   scanf("%d", &n);
   if(n%2 != 0 && n%2 > 0){
      triangle(n);
      //printf("%d\n", n);
   }else{
      printf("ERROR - TRY AGAIN!\n");   
}
}
return 0;
}



   





*/
