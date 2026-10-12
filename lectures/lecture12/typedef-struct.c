/* typedef-struct.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* typedef ... <type-name> */

typedef struct
{
  unsigned int rin;   /* e.g., 660000001 */
  char rcsid[20];     /* e.g., "goldsd3" */
  /* ... */
}
student_t;


int main()
{
  printf( "sizeof student_t is %lu\n", sizeof( student_t ) );

  /* static allocation */
  student_t me;
  me.rin = 660000001;
  strcpy( me.rcsid, "goldsd3" );

  printf( "me: RIN %u; RCSID %s\n", me.rin, me.rcsid );

  /*************************************************************/

  /* dynamic allocation */
  student_t * st;
  st = malloc( sizeof( student_t ) );   /* e.g., 24 bytes */
  (*st).rin = 662123456;                /* or: st->rin */
  strcpy( (*st).rcsid, "smithj123" );

  printf( "st: RIN %u; RCSID %s\n", (*st).rin, (*st).rcsid );
  printf( "st: RIN %u; RCSID %s\n", st->rin, st->rcsid );

  free( st );

  /*************************************************************/

  int num_of_undergrads = 6000;

  /* dynamically allocate an array of student_t of size 6000 UGs */
  student_t * students = calloc( num_of_undergrads, sizeof( student_t ) );


  /* is this a "shallow copy" or a "deep copy" of me into students[123]? */
  /* ANSWER: all 24 bytes are copied... */
  students[123] = me;
  *( students + 123 ) = me;

  me.rin = 660000007;   /* we are only changing the variable me here... */

  printf( "students[123]: RIN %u; RCSID %s\n",
          students[123].rin, students[123].rcsid );

  printf( "students[123]: RIN %u; RCSID %s\n",
          (*( students + 123 )).rin, (*( students + 123 )).rcsid );

  free( students );

  return EXIT_SUCCESS;
}

#if 0
  *
  *  typedef struct
  *  {
  *    unsigned int rin;   /* e.g., 660000001 */
  *    char * rcsid;       /* e.g., "goldsd3" */
  *  }
  *  student_t;
  *
  *  The above struct requires 12 bytes, and an assignment
  *   statement would copy only those 12 bytes (shallow copy)
  *
#endif



