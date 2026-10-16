/* Hw2 */
/*  gcc -Wall -Werror check3-writer.c -o check3-writer.out */
/* mkfifo /tmp/check3-writer */
/*  ./check3-writer.out -f output.txt */
/* ./hw2r.out input.txt unused-read unused-write*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

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
  return index; }

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
#if 1
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
   /* Sixteen letters plus the ending '\0'. */
   char *word = calloc(MAXWORDLENGTH + 1, sizeof(char));
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

   /* Read one byte at a time. Each newline finishes one word. */
   while ((r = read(fd, &byte, 1)) > 0) {
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
      else if (good && length < MAXWORDLENGTH &&
               ((byte >= 'A' && byte <= 'Z') ||
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

   /* Also handle a final word when the file has no final newline. */
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

   free(word);
   if (close(fd) == -1) {
      perror("ERROR: close");
      return -1;
   }

   return wc;
}

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
void free_trie( trie_t * trie ) {
  if(trie == NULL) return;  /* safeguard against NULL pointer */
  for(int i = 0; i < 26; i++){
    free_trie((trie + i)->next);
  }
  free(trie);
 }



int main(int argc, char ** argv){
   setvbuf(stdout, NULL, _IONBF, 0);
   if (argc != 4){
      fprintf(stderr," USAGE: hw2.out<dictionary-file> <read-fifo> <write-fifo> \n");
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

   /* Temporary test: remove this call */
   ///print_trie(trie);

   int fd = open(*(argv + 2), O_RDONLY); // reading part of the FIFO
   if( fd == -1){perror( "reading open() failed"); return EXIT_FAILURE; }
   int fd2 = open(*(argv + 3), O_WRONLY); // Writing part of the FIFO
   if( fd2 == -1) {perror ("writing open() failed"); return EXIT_FAILURE; }

   char * candidate = calloc(MAXWORDLENGTH, sizeof(char));
   if (candidate == NULL){perror("calloc() failed"); return EXIT_FAILURE; }
   const int byte = read(fd, candidate, MAXWORDLENGTH);
   int len = strlen((int)byte);
   int status; // status exit
   pid_t p; // for process id of parent and child used in fork
   pid_t w; // for process id of waitpid only used in parent 
   char response; // response of child wether it adds a '.' '>' or '?'
   int bw = write(fd2, (char)response, 1);
   while (fd == 1){ // while the reading and returning bytes
      for(int i = 0; *((int)byte + i); i++){
         if( byte == atoi((int)byte) ){
            strcpy(&byte ,candidate);
         }
         if (byte == '-'){
            p = fork();
            if (p == -1){perror("fork() failed"); return EXIT_FAILURE;}
            }
            if(p == 0){
               close(fd);
               upcase_word(byte, len);
               if(is_valid_word(trie, byte, len) == TRUE){
               response = byte + '.';
               //int bw = write(fd2, (char)response, 1);
               }else{
                  char alertnative = find_alternative(candidate, trie);
                  if(find_alterntive(candidate, trie) == TRUE){
                     response = byte + '>';
                  }else{
                     response = byte + '?';
                  }
               }
               printf("CHILD: response: %c", bw);
               close(fd2);
               exit(status);
            }

            if(p > 0){
               close(fd);
               close(fd2);
               candidate = NULL;
               w = waitpid(p, &status, 0);
               free_trie(trie);
               free(candidate);
               return EXIT_SUCCESS;
               
            }
      }
}
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
