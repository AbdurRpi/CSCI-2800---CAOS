/* lab06-checkpoint1.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#define MAXWORDLENGTH 32

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
  free( prefix );
}


/* use the toupper() function of ctype.h to convert the
 *  given word to UPPERCASE
 *
 * (OPTIONAL) implement this function using only pointers,
 *             i.e., do not use any int variables or wordlen
 *
 * (OPTIONAL) implement this function without using any
 *             new variables at all, i.e., only use word
 *
 *            (see the command-line-args-v2.c example)
 */
void upcase_word( char * word, int wordlen ) {
  for(int i = 0; i < wordlen; i++){
    char letter = *(word + i);
    letter = toupper(letter);
    //
    strcpy(&letter, word);
  }
 }

/* map the given UPPERCASE letter to an index,
 *  i.e., 'A' maps to index 0, 'B' maps to index 1, etc.
 *
 * (HINT: you can achieve this with one simple calculation)
 */
int trie_index( char letter ) {
  int index = letter - 'A';
  return index; }



/* add the given word to the Trie data structure,
 *  expanding the data structure as necessary
 *
 * be sure to dynamically allocate memory for the word
 *  and also each new Trie layer that you add
 *
 * this function returns the length of the word added
 *   or -1 if something goes wrong
 *
 * (OPTIONAL) use recursion to implement this function
 */
int add_word( trie_t * trie, const char * word, int wordlen ) { 
  for(int i = 0; *(word + i + 1) != '\0'; i++){
     trie = calloc(wordlen, sizeof(char));
    strcpy(word, trie);
    print_trie_helper( (*(trie + i)).next, word );
    return wordlen;
    if(wordlen > MAXWORDLENGTH){
      return -1
    }
  }

  }
  /* create as many allocation for the value of wordlen, if the allocations already exist dont reallocate
  * .next points to the next layer if the layer doesnt exist then create it.
  * every layer of the array should be allocated to the 26 spaces by alphabet default.
  * then return.
  *
  */


/* given a word in UPPERCASE, determine if it is valid
 *  according to the given Trie data structure;
 *   return 1 if valid or 0 otherwise
 *
 * (OPTIONAL) use recursion to implement this function
 */
int is_valid_word( const trie_t * trie, const char * word, int wordlen ) { return 0; }


/* deallocate all dynamically allocated memory for
 *  the given Trie data structure
 *
 * (OPTIONAL) use recursion to implement this function
 */
void free_trie( trie_t * trie ) { }


int main( int argc, char ** argv )
{
  /* array to hold top layer of the Trie */
  trie_t trie[26];

  /* add words to the Trie; modify/expand as you like */
  add_word( trie, "AA", 2 );
  add_word( trie, "AAH", 3 );
  add_word( trie, "AAHED", 5 );
  add_word( trie, "AAHING", 6 );
  add_word( trie, "AAHS", 4 );
  add_word( trie, "AAL", 3 );
  add_word( trie, "AALII", 5 );
  add_word( trie, "AALIIS", 6 );
  add_word( trie, "AALS", 4 );
  add_word( trie, "AARDVARK", 8 );
  add_word( trie, "CAOS", 4 );

  printf( "Trie contains valid words:\n" );
  print_trie( trie );

  /* loop to repeatedly ask the user to input a word to check */
  while ( 1 )
  {
    char buffer[MAXWORDLENGTH + 2];
    printf( "Enter word to check (CTRL-D to exit): " );
    if ( fgets( buffer, MAXWORDLENGTH + 1, stdin ) == NULL ) break;
    buffer[strlen( buffer ) - 1] = '\0';  /* remove '\n' from input */
    upcase_word( buffer, strlen( buffer ) );
    int rc = is_valid_word( trie, buffer, strlen( buffer ) );
    printf( "==> %s is %s\n", buffer, rc == 1 ? "valid" : "not valid" );
  }

  printf( "\n" ); 

  /* free up all dynamically allocated memory for the given Trie */
  free_trie( trie );

  return EXIT_SUCCESS;
}
