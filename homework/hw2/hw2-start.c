/* hw2-start.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAXWORDLENGTH 16


typedef struct trie
{
  char valid;         /* 0 if not a valid word to this point; 1 if word is valid */
  struct trie * next; /* pointer to next level of Trie; or NULL if no next level */
}
trie_t;


void print_trie_helper( const trie_t * trie, char * prefix )
{
  const trie_t * ptr = trie;

  /* visit each of the 26 elements of this layer of the Trie */
  for ( int i = 0 ; i < 26 ; i++ )
  {
    int len = strlen( prefix );

    /* append next character */
    *(prefix + len) = 'A' + i;

    /* display valid word, if present */
    if ( (*(ptr + i)).valid ) printf( "==> %s\n", prefix );

    /* recurse to the next level, if present */
    if ( (*(ptr + i)).next ) print_trie_helper( (*(ptr + i)).next, prefix );

    /* remove last character */
    *(prefix + len) = '\0';
  }
}


/* Display all words in the given Trie */
void print_trie( const trie_t * trie )
{
  if ( trie == NULL ) return;  /* safeguard against NULL pointer */
  char * prefix = calloc( MAXWORDLENGTH + 1, sizeof( char ) );
  if ( prefix == NULL ) { perror( "calloc() failed" ); exit( EXIT_FAILURE ); }
  print_trie_helper( trie, prefix );
}
