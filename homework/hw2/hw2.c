/* Hw2 */
/*  gcc -Wall -Werror check3-writer.c -o check3-writer.out */
/* mkfifo /tmp/check3-writer */
/*  ./check3-writer.out -f output.txt */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

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
void load_dictionary(filename, trie){
   char letter = *(prefix);
   int wc = 0;
   int r = read(filename, O_RDONLY, sizeof(char));
   char **buffer = calloc(MAXWORDLENGTH + 1, sizeof(char));

   while(r > 0){
      if(byte == letter){
         upper(byte);
         if (word <= MAXWORDLENGTH){
            strcpy(word, buffer);
         }
      }else if(byte == '\n'){
         if(word == word.valid){
            add_word(word);
            wc += 1
            free(buffer);
         }
      }else{
         int fd = open(filename);
         fd[0] == !word.valid;
      }
      if (EOF || (!'\n')){
         continue;
      }
   }
   close(filename);
   return wc;
}

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


int main(int argc, ** argv){
   if (argv != 4){
      fprintf(stderr," USAGE: hw2.out<dictionary-file> <read-fifo> <write-fifo> \n");
      return EXIT_FAIURE;
   }
}

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

