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
  for (int i = 0; i < wordlen; i++) {
    char letter = *(word + i);
    letter = (char)toupper((unsigned char)letter);
    *(word + i) = letter;
  }
}


/* map the given UPPERCASE letter to an index,
 *  i.e., 'A' maps to index 0, 'B' maps to index 1, etc.
 *
 * (HINT: you can achieve this with one simple calculation)
 */
int trie_index( char letter ) {
  int index = letter - 'A';
  return index;
}


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
  if (trie == NULL || word == NULL || wordlen < 1 ||
      wordlen > MAXWORDLENGTH || strlen(word) != (size_t)wordlen) return -1;

  for (int i = 0; *(word + i) != '\0'; i++) {
    int index = trie_index(*(word + i));
    if (index < 0 || index >= 26) return -1;

    trie_t *current = trie + index;
    if (i == wordlen - 1) {
      current->valid = 1;
    } else {
      if (current->next == NULL) {
        current->next = calloc(26, sizeof(trie_t));
        if (current->next == NULL) return -1;
      }
      trie = current->next;
    }
  }
  return wordlen;
}


/* given a word in UPPERCASE, determine if it is valid
 *  according to the given Trie data structure;
 *   return 1 if valid or 0 otherwise
 *
 * (OPTIONAL) use recursion to implement this function
 */
int is_valid_word( const trie_t * trie, const char * word, int wordlen ) {
  if (trie == NULL || word == NULL || wordlen < 1 ||
      wordlen > MAXWORDLENGTH || strlen(word) != (size_t)wordlen) return 0;

  for (int i = 0; i < wordlen; i++) {
    int index = trie_index(*(word + i));
    if (index < 0 || index >= 26) return 0;

    const trie_t *current = trie + index;
    if (i == wordlen - 1) return current->valid;
    if (current->next == NULL) return 0;
    trie = current->next;
  }
  return 0;
}

/* Like print_trie_helper, but collect valid words instead of printing. */
void collect_words(const trie_t *trie, char *prefix, char **result) {
  const trie_t *ptr = trie;

  for (int i = 0; i < 26; i++) { //traverse a -z
    int len = (int)strlen(prefix); 
    if (len >= MAXWORDLENGTH) return; // error check for word lenthg to long

    *(prefix + len) = (char)('A' + i); // like print try helper it, this adds a node letter to the end of the prefix

    if ((*(ptr + i)).valid) { // if letter is valid 
      /* Leave space for the old list, comma and space, new word, and '\0'. */
      int size = strlen(*result) + strlen(prefix) + 3;
      char *larger = realloc(*result, size);
      if (larger == NULL) { perror("realloc() failed"); exit(EXIT_FAILURE); }
      *result = larger;
      if (**result != '\0') strcat(*result, ", ");
      strcat(*result, prefix);
    }

    if ((*(ptr + i)).next != NULL)
      collect_words((*(ptr + i)).next, prefix, result);

    *(prefix + len) = '\0';
  }
}

char *search_prefix(const trie_t *trie, const char *prefix, int prefixlen) {
  if (trie == NULL || prefix == NULL || prefixlen < 0 ||
      prefixlen > MAXWORDLENGTH || (size_t)prefixlen > strlen(prefix)) return NULL;

  char *word = calloc(MAXWORDLENGTH + 1, sizeof(char));
  if (word == NULL) return NULL;

  const trie_t *current = NULL;
  for (int i = 0; i < prefixlen; i++) {
    int index = trie_index(*(prefix + i));
    if (index < 0 || index >= 26) { free(word); return NULL; }

    current = trie + index;
    *(word + i) = *(prefix + i);
    if (i < prefixlen - 1) {
      if (current->next == NULL) { free(word); return NULL; }
      trie = current->next;
    }
  }

  char *result = calloc(MAXWORDLENGTH + 1, sizeof(char));
  if (result == NULL) { free(word); return NULL; }

  if (prefixlen == 0) {
    collect_words(trie, word, &result);
  } else {
    if (current->valid) strcpy(result, word);
    if (current->next != NULL) collect_words(current->next, word, &result);
  }
  free(word);

  if (*result == '\0') { free(result); return NULL; }
  return result;
}

/* Choose the first word under the longest matching prefix. */
char *suggest_word(const trie_t *trie, const char *word, int wordlen) {
  if (trie == NULL || word == NULL || wordlen < 1 ||
      wordlen > MAXWORDLENGTH || strlen(word) != (size_t)wordlen ||
      is_valid_word(trie, word, wordlen)) return NULL;

  for (int i = wordlen; i >= 0; i--) {
    char *result = search_prefix(trie, word, i);
    if (result != NULL) {
      for (int j = 0; *(result + j) != '\0'; j++) {
        if (*(result + j) == ',') {
          *(result + j) = '\0';
          break;
        }
      }
      return result; /* Caller frees the returned string. */
    }
  }
  return NULL;
}


/* deallocate all dynamically allocated memory for
 *  the given Trie data structure
 *
 * (OPTIONAL) use recursion to implement this function
 */
void free_trie( trie_t * trie ) {
  if (trie == NULL) return;
  for (int i = 0; i < 26; i++)
    free_trie((trie + i)->next);
  free(trie);
}


int main( int argc, char ** argv )
{
  /* array to hold top layer of the Trie */
  trie_t *trie = calloc(26, sizeof(trie_t));
  if (trie == NULL) { perror("calloc() failed"); return EXIT_FAILURE; }

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

  /* Allocate input once so no square bracket notation is needed. */
  char *buffer = calloc(MAXWORDLENGTH + 2, sizeof(char));
  if (buffer == NULL) {
    perror("calloc() failed");
    free_trie(trie);
    return EXIT_FAILURE;
  }

  /* loop to repeatedly ask the user to input a word to check */
  while ( 1 )
  {
    printf( "Enter word to check (CTRL-D to exit): " );
    fflush(stdout);
    if ( fgets( buffer, MAXWORDLENGTH + 2, stdin ) == NULL ) break;

    size_t length = strlen(buffer);
    if (length > 0 && *(buffer + length - 1) == '\n') {
      *(buffer + length - 1) = '\0';
      length--;
    } else if (length > MAXWORDLENGTH) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF) { }
    }
    if (length < 1 || length > MAXWORDLENGTH) {
      printf("==> enter 1 to %d letters\n", MAXWORDLENGTH);
      continue;
    }

    upcase_word( buffer, (int)length );
    int rc = is_valid_word( trie, buffer, (int)length );
    if (rc == 1) {
      printf("==> %s is valid\n", buffer);
    } else {
      printf("==> %s is not valid", buffer);
      char *alternate = suggest_word(trie, buffer, (int)length);
      if (alternate != NULL) {
        printf("; did you mean %s?", alternate);
        free(alternate);
      }
      printf("\n");
    }
  }

  printf( "\n" ); 

  free(buffer);
  /* free up all dynamically allocated memory for the given Trie */
  free_trie( trie );

  return EXIT_SUCCESS;
}
