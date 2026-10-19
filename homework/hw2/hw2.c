/* Hw2 */
/*   gcc -Wall -Werror hw2.c -o hw2.out  */
/* mkfifo /tmp/hw2-read /tmp/hw2-write */

/* ./hw2r.out input.txt /tmp/hw2-read /tmp/hw2-write  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>

#define MAXWORDLENGTH 16

typedef struct trie{
   char valid;
   //char layer;
   struct trie *next;
}
trie_t;

void print_trie_helper(const trie_t *trie, char *prefix){
   const trie_t *ptr = trie;

   for(int i = 0; i < 26; i++){ /* visit each of the 26 elements of this layer of the Trie */
      int len = strlen(prefix); 
      *(prefix + len) = 'A' + i; // append next letter
      if( (*(ptr + i)).valid )
         printf("==> %s\n", prefix);
      if((*(ptr + i)).next)
         print_trie_helper((*(ptr + i)).next, prefix); // recurse to the next level if present

      *(prefix + len) = '\0';
   }
}

void print_trie(const trie_t * trie){
   if (trie == NULL) return;
   char * prefix = calloc( MAXWORDLENGTH + 1, sizeof(char));
   if ( prefix == NULL) {perror("calloc() failed"); exit(EXIT_FAILURE);}
   print_trie_helper( trie, prefix);
   free(prefix);
}

void upcase_word( char * word, int wordlen ) {
  for(int i = 0; i < wordlen; i++){
    char letter = *(word + i);
    letter = toupper(letter);
    *(word + i) = letter;
    //
    //strcpy(&letter, word);
  }
 }

int trie_index( char letter ) {
  int index = letter - 'A';
  return index; 
}

int add_word( trie_t * trie, const char * word, int wordlen ) { 
  if( wordlen < 1 || wordlen > MAXWORDLENGTH) return -1;  /* safeguard against invalid value */
  for(int i = 0; *(word + i ) != '\0'; i++){ // Check if the last letter of the word is backdlash 0
     //if ( !(*(trie + i)).next || i < wordlen){ // if Not next layer ot i less then worlden
      int index = trie_index(*(word + i)); // uses the previous function to get index of a letter
      if (index < 0 || index >= 26) return -1;
       trie_t *current = trie + index; // find letter in current layer 
      if(i == wordlen -1){  /* if last letter last position is valid*/
        current->valid = 1;
      }else{
        if (current->next == NULL){ /* if layer doesnt exist  create new layer*/
          current->next = calloc(26, sizeof(trie_t));
          if (current->next == NULL) return -1;
          printf("PARENT: creating new Trie layer for '%c' of \"%s\"\n", *(word + i), word);
        }
        trie = current->next;  /* move down to next layer*/
      }
   }
   return wordlen;
}

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

int load_dictionary(const char *filename, trie_t *trie){
   int fd = open(filename, O_RDONLY);
   if (fd == -1) {
      perror("ERROR: open");
      return -1;
   }
   
   char *word = calloc(MAXWORDLENGTH + 1, sizeof(char)); /* Sixteen letters plus the ending '\0'. */
   if (word == NULL) {
      perror("ERROR: calloc");
      close(fd);
      return -1;
   }
   int wc = 0;          /* Number of words added */
   int length = 0;      /* Letters collected on this line */
   int good = 1;        /* Whether this line is still a valid word */
   char byte;
   ssize_t r;

   
   while ((r = read(fd, &byte, 1)) > 0) { /* Read one byte at a time. Each newline finishes one word. */
      if (byte == '\n') {
         if (good && length > 0) {
            *(word + length) = '\0';
            upcase_word(word, length);

            if (add_word(trie, word, length) == -1) {
               fprintf(stderr, "ERROR: could not add dictionary word\n");
               free(word);
               close(fd);
               return -1;
            }
            wc++;
         }
         /* Start collecting the next line. */
         length = 0;
         good = 1;
      }
      else if (good && length < MAXWORDLENGTH && ((byte >= 'A' && byte <= 'Z') || 
      (byte >= 'a' && byte <= 'z'))) {
         *(word + length) = byte;
         length++;
      }
      else {
         /* Invalid character or more than 16 letters:
          * skip this line until its newline.
          */
         good = 0;
      }
   }
   if (r == -1) {
      perror("ERROR: read");
      free(word);
      close(fd);
      return -1;
   }
   if (good && length > 0) { /* Also handle a final word when the file has no final newline. */
      *(word + length) = '\0';
      upcase_word(word, length);

      if (add_word(trie, word, length) == -1) {
         fprintf(stderr, "ERROR: could not add dictionary word\n");
         free(word);
         close(fd);
         return -1;
      }
      wc++;
   }

   free(word);
   if (close(fd) == -1) {
      perror("ERROR: close");
      return -1;
   }
   return wc;
}

void free_trie( trie_t * trie ) {
  if(trie == NULL) return;  /* safeguard against NULL pointer */
  for(int i = 0; i < 26; i++){
    free_trie((trie + i)->next);
  }
  free(trie);
 }

/* Search the current Trie layer after index, wrapping past Z to A.
 * Return the position of the first valid word, or -1 if none exists.
 */
int find_next_valid(const trie_t *trie, int index)
{
   for (int i = 1; i <= 26; i++) {
      int position = index + i;

      if (position >= 26)
         position -= 26;

      if ((trie + position)->valid)
         return position;
   }

   return -1;
}

/* Find an alternate at the deepest usable Trie layer.
 * The returned word is allocated with calloc; the child frees it.
 */
char *suggest_word(const trie_t *trie, const char *word, int wordlen)
{
   if (trie == NULL || word == NULL || wordlen < 1)
      return NULL;

   const trie_t *layer = trie;
   int best_depth = -1;
   int best_index = -1;

   for (int i = 0; i < wordlen && i < MAXWORDLENGTH; i++) {
      int index = trie_index(*(word + i));
      if (index < 0 || index >= 26)
         break;

      
      int position = find_next_valid(layer, index); /* Looks for a valid word at this layer, starting after
       * the candidate's letter and wrapping around if needed.
       */

      if (position != -1) {
         best_depth = i;
         best_index = position;
      }
      /* Follow the candidate's path if another letter remains. */
      if (i == wordlen - 1)
         break;

      const trie_t *current = layer + index;
      if (current->next == NULL)
         break;

      layer = current->next;
   }

   if (best_depth == -1)
      return NULL;

   /* Copies the matching part of the candidate, then add the
    * letter found at the deepest layer. Including room for '\0'.
    */
   char *alternative = calloc(best_depth + 2, sizeof(char));
   if (alternative == NULL) {
      perror("ERROR: calloc");
      _exit(EXIT_FAILURE);
   }

   for (int i = 0; i < best_depth; i++)
      *(alternative + i) = *(word + i);

   *(alternative + best_depth) = 'A' + best_index;
   return alternative;
}

int main(int argc, char ** argv){
   setvbuf(stdout, NULL, _IONBF, 0);
   if (argc != 4){
      fprintf(stderr, "USAGE: hw2.out <dictionary-file> <read-fifo> <write-fifo>\n");
      return EXIT_FAILURE;
   }

trie_t *trie = calloc(26, sizeof(trie_t));
if(trie == NULL){
   perror("ERROR: calloc");
   return EXIT_FAILURE;
}

 int word_count = load_dictionary(*(argv + 1), trie);
   if (word_count == -1) {
      free_trie(trie);
      return EXIT_FAILURE;
   }

   printf("PARENT: added %d words to the Trie\n", word_count);

   /* Temporary test  */
   ///print_trie(trie);

   int capacity = MAXWORDLENGTH + 1; // extra space for \0
   int len = 0; // variable for counting the length of bytes coming in

   char * candidate = calloc(capacity, sizeof(char)); // buffer for inputted words
   if (candidate == NULL){
      perror("calloc() failed");
      //close(fd);
      //close(fd2);
      free_trie(trie);
    return EXIT_FAILURE; 
   }

   int fd = open(*(argv + 2), O_RDONLY); // reading part of the FIFO
   if( fd == -1){
      perror( "ERROR: reading open() failed"); 
      free(candidate);
      free_trie(trie);
      return EXIT_FAILURE; 
   }

   int fd2 = open(*(argv + 3), O_WRONLY); // Writing part of the FIFO
   if( fd2 == -1) {
      perror ("ERROR: writing open() failed"); 
      close(fd);
      free(candidate);
      free_trie(trie);
      return EXIT_FAILURE; 
   }

   //const int byte = read(fd, candidate, MAXWORDLENGTH);
   char byte;
   int status = EXIT_SUCCESS; // exit status 
   int cstatus; // status for waitpid
   pid_t p; // for process id of parent and child used in fork
   pid_t w; // for process id of waitpid only used in parent 
   //char response; // response of child wether it adds a '.' '>' or '?'
   int r;
   int children = 0;

   while ((r = read(fd, &byte, 1)) > 0){ // while the reading and returning bytes, reads one character at a time

         if (byte != '-'){
            if (len + 1 >= capacity){ // This function is in case of an overflow, if candidate exceeds wordlen
               int ncap = len + 2;
               char *buffer = calloc(ncap, sizeof(char));
               if (buffer == NULL){
                  perror("ERROR: calloc()");
                  status = EXIT_FAILURE;
                  break;
               }
               strcpy(buffer, candidate);
               free(candidate);
               candidate = buffer;
               capacity = ncap;
            }
            *(candidate + len) = byte;
            len ++;
            *(candidate + len) = '\0';
            continue;
         }
            if (len == 0) continue; //

            p = fork();
            if (p == -1){
               perror("ERROR: fork");
               status = EXIT_FAILURE;
               break;
            }

            if (p == 0) {
               close(fd);
               upcase_word(candidate, len);

               int valid = 0;
               if (len <= MAXWORDLENGTH) {
                  valid = is_valid_word(trie, candidate, len);
               }

               char *alternative = NULL;
               if (!valid) {
                  alternative = suggest_word(trie, candidate, len);
               }

               char *response = calloc(len + MAXWORDLENGTH + 3, sizeof(char));
               if (response == NULL) {
                  perror("ERROR: calloc");
                  free(alternative);
                  free(candidate);
                  free_trie(trie);
                  close(fd2);
                  _exit(EXIT_FAILURE);
               }

               strcpy(response, candidate);

               if (valid) {
                  *(response + len) = '.';
               }
               else if (alternative != NULL) {
                  *(response + len) = '>';
                  strcpy(response + len + 1, alternative);
                  int end = strlen(response);
                  *(response + end) = '?';
               }
               else {
                  *(response + len) = '?';
               }

               int response_len = strlen(response);
               int bw = write(fd2, response, response_len);
               int child_status = EXIT_SUCCESS;

               if (bw != response_len) {
                  if (bw == -1)
                     perror("ERROR: write");
                  else
                     fprintf(stderr, "ERROR: incomplete fifo response\n");

                  child_status = EXIT_FAILURE;
               }

               free(response);
               free(alternative);
               free(candidate);
               free_trie(trie);
               close(fd2);
               _exit(child_status);
            }
       
            if(p > 0){
               children++;
               len = 0;
               *candidate = '\0';

               while (children > 0) {  /* Check for finished children without stopping for running ones. */
                  w = waitpid(-1, &cstatus, WNOHANG);
                  if (w > 0) {
                     children--;
                     if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus) != EXIT_SUCCESS){
                        status = EXIT_FAILURE;
                     }
                  }else if (w == 0) {
                     break;
                  }else {
                     perror("ERROR: waitpid");
                     status = EXIT_FAILURE;
                     break;
                  }
               }
               if (status == EXIT_FAILURE) break;
            }
}
            if (r == -1) {
               perror("ERROR: read");
               status = EXIT_FAILURE;
            }
            if (close(fd) == -1) {
               perror("ERROR: close read fifo");
               status = EXIT_FAILURE;
            }
            if (close(fd2) == -1) {
               perror("ERROR: close write fifo");
               status = EXIT_FAILURE;
            }

            free(candidate);

            /* Wait for children that were still running when reading ended. */
            while (children > 0) {
               w = waitpid(-1, &cstatus, 0);

               if (w == -1) {
                  perror("ERROR: waitpid");
                  status = EXIT_FAILURE;
                  break;
               }
               children--;
               if (!WIFEXITED(cstatus) || WEXITSTATUS(cstatus) != EXIT_SUCCESS)
                  status = EXIT_FAILURE;
            }
            free_trie(trie);
            return status;
}
      

#if 0
setvbuff(stdout, NULL, _IONBF, 0);

trie_t * tt;

int nums = 26;
char f;
int filename = scanf("%c", &f);
tt = calloc(nums, sizeof(char)); 

int fd = open(filanme, O_RDONLY, );

(*tt).layer = *


load_dictionary(filename, trie);

printf("PARENT: creating new Trie layer for ")
#endif

#if 0
void add_word(trie_t trie, const char word){
   trie_t *layer = trie;
   for(int i = 0; *(word + i) != '\0'; i++){
      char letter = *(word + i);
      int position = letter - 'A';
      trie_t *node = layer + position;

      if (*(word + i + 1) == '\0'){
         node-> valid = 1;
      } else{
         if (node-> next == NULL){
            node-> next = calloc(26, sizeof(trie_t));
            if (node-> next == NULL){
               perror("ERROR: called() failed");
               return -1;
            }
            printf("PARENT: creating new Trie layer for '%c' of \"%s\"\n", letter, word);
         }
         layer = node->next;
      }
}
return 0;
}
#endif
#if 0
void load_dictionary(filename, trie){
   int fd = open(filename, O_RDONLY);
   if (fd == -1) {perror("open failed"); return EXIT_FAILURE;}
   int n = read(filename, *prefix, sizeof(char));
   if( n == -1){perror("read() failed");}
   for((*(word) == prefix.valid)){
      add_word(trie, word);
      word.upper;
      close(filename);
   }
   return word.valid;
}
#endif

#if 0
void read_candidate(read_fifo){ // continue here where you left off. NEED HELP
   int fd = open(fifo, word, sizeof(char));
   while(!(*(prefix + i)).valid){

      char **candidate = calloc(*(prefix) + i, sizeof(char));

      
      if(*(prefix + i) == (*(prefix + i) + '-')){
         break;
      }
      strcpy(word, candidate);
      return candidate
   }
   free(candidate);
}


void make_response(trie, candidate){

}
#endif