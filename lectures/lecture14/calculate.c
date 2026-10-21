/* calculate.c */

/* Use the -S flag to generate assembly code, though
 *  note that the resulting calculate.s is not MIPS!
 *
 *
 * bash$ gcc -S calculate.c
 */

void main()
{
  int g = 7, h = 6, i = 5, j = 2;

  int f = g + h - i - j;

}
