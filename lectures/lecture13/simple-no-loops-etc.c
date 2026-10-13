/* simple-no-loops-etc.c */

/* compile via:
 *
 * bash$ gcc -Wall -Werror simple-no-loops-etc.c -lm
 *
 * bash$ gcc -E -Wall -Werror simple-no-loops-etc.c -lm   <== preprocessor ONLY
 *
 * bash$ gcc -S -Wall -Werror simple-no-loops-etc.c -lm   <== generate assembly (.s file)
 */

#include <stdio.h>
#include <stdlib.h>  /* #define EXIT_SUCCESS 0 */
#include <unistd.h>
#include <math.h>    /* gcc ... -lm <== link the math library */

int main()
{
  int x;

  /* GOAL: remove the for loop and use labels and goto instead... */

#if 0
  /* original code using a higher-level language for loop */
  for ( x = 1 ; x < 8 ; x++ )
  {
    printf( "%3d %f %20.15f\n", x, sqrt( x ), sqrt( x ) );
  }
#endif

#if 0
  /* modified code using a higher-level language while loop */
  x = 1;
  while ( x < 8 )
  {
    printf( "%3d %f %20.15f\n", x, sqrt( x ), sqrt( x ) );
    x++;
  }
#endif

#if 0
  /* same loop as above using labels and goto statements,
   *  and the loop condition check below the loop body
   */
  x = 1;
  goto ENDLOOP1;
LOOP1:
  printf( "%3d %f %20.15f\n", x, sqrt( x ), sqrt( x ) );
  x++;
ENDLOOP1:
  if ( x < 8 ) goto LOOP1;
#endif

#if 0
  /* same loop as above using labels and goto statements,
   *  this time checking the loop condition above the loop body
   */
  x = 1;
LOOP1:
  if ( x >= 8 ) goto DONE1;
  printf( "%3d %f %20.15f\n", x, sqrt( x ), sqrt( x ) );
  x++;
  goto LOOP1;
DONE1:
#endif

#if 1
  /* same loops as above, this time separating out the condition
   *  from the if statement
   */
  x = 1;
LOOP1:
  int tmp = ( x >= 8 );
  if ( tmp ) goto DONE1;   /* "branch" or "conditional jump" instruction */
  printf( "%3d %f %20.15f\n", x, sqrt( x ), sqrt( x ) );
  x++;
  goto LOOP1;
DONE1:
#endif

  /* compound calculations need to be separated out into individual steps
   *
   * e.g., int result = 8 * x + 512;
   *
   *       ==> int tmp = 8;
   *           tmp = tmp * x;
   *           tmp = tmp + 512;
   */

  /* TO DO: Write a C program to convert a binary value that the user
   *         types in, e.g., 10101111, into its decimal equivalent,
   *          e.g., 128 + 32 + 8 + 4 + 2 + 1 = 175 (double-check...)
   *
   *        Next, modify your code to not use loops (use labels/goto)
   *
   *        Also, modify your code to only use if statements with if ( var );
   *         in other words, all conditional logic must be in var
   *
   *        Perform all calculations one step at a time...
   */

  return EXIT_SUCCESS;
}
