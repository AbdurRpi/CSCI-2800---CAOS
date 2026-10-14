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
    *(word + i) = letter;
    //
    //strcpy(&letter, word);
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
  if( wordlen < 1 || wordlen > MAXWORDLENGTH) return -1;  /* safeguard against invalid value */
  for(int i = 0; *(word + i ) != '\0'; i++){ // Check if the last letter of the word is backdlash 0
     //if ( !(*(trie + i)).next || i < wordlen){ // if Not next layer ot i less then worlden
      int index = trie_index(*(word + i)); // uses the previous function to get index of a letter
      trie_t *current = trie + index; // find letter in current layer 
      if (index < 0 || index >= 26) return -1;
      if(i == wordlen -1){  /* if last letter last position is valid*/
        current->valid = 1;
      }else{
        if (current->next == NULL){ /* if layer doesnt exist  create new layer*/
          current->next = calloc(26, sizeof(trie_t));
          if (current->next == NULL) return -1;
        }
        trie = current->next;  /* move down to next layer*/
      }
  
}
return wordlen;
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
int is_valid_word( const trie_t * trie, const char * word, int wordlen ) {
  if(wordlen < 1 || wordlen > MAXWORDLENGTH) return 0; 
  for (int i = 0; i < wordlen; i++) {
    int index = trie_index(*(word + i));
    if (index < 0 || index >= 26) return 0;

    const trie_t *current = trie + index;

    /* The final letter marks the end of a complete word. */
    if (i == wordlen - 1) return current->valid;

    /* if more letters remain there is another layer. */
    if (current->next == NULL) return 0;
    trie = current->next;
  }

  return 0;
}

  #if 0
  if(wordlen < 1 || wordlen > MAXWORDLENGTH) return 0; 
  for(int i = 0; i < wordlen; i++){
    if((trie + i)->valid){
      return 1;
    }
  }
  return 0; }
  #endif


/* given a word in UPPERCASE that is NOT valid according
* to the given Trie data structure, determine an alternate
* word from the Trie; consider using a helper function
* that iterates through the current Trie layer
*
* function returns a pointer to the alternate word
* or NULL if no alternate is found
*
* [OPTIONAL] use recursion to implement this function */
#if 0
char * suggest_word( const trie_t * trie, const char * word, int wordlen ){
  for(int i = 0; i < upcase_word(word, i); i++){
    if( !is_valid_word(word, i)){
      trie_index(word, i){}


    }

  }
}

#endif

/* given a prefix in UPPERCASE, generate a list of all valid
* words according to the given Trie data structure; format
* the output as a comma-separated list; consider using
* a helper function and possibly recursion
*
* function returns a pointer to a list of all valid words
* with the given prefix or NULL if no words with the
* given prefix exist
*
* note that this function must dynamically allocate memory
* for the output string; it is up to the caller to properly
* de-allocate memory for this string
*
* [OPTIONAL] use recursion to implement this function
*/
//char * search_prefix( const trie_t * trie, const char * prefix, int prefixlen ){
  
//}

/* deallocate all dynamically allocated memory for
 *  the given Trie data structure
 *
 * (OPTIONAL) use recursion to implement this function
 */

void free_trie( trie_t * trie ) {
  if(trie == NULL) return;  /* safeguard against NULL pointer */
  for(int i = 0; i < 26; i++){
    free_trie((trie + i)->next);
  }
  free(trie);
 }


int main( int argc, char ** argv )
{
  /* array to hold top layer of the Trie */
  trie_t *trie = calloc(26, sizeof(trie_t));
  if (trie == NULL){ perror("calloc() failed"); return EXIT_FAILURE;}

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
  //free(buffer);

  return EXIT_SUCCESS;
}
